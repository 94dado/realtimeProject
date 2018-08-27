/*
24_cooktorrance_tex_dirlight.frag: fragment shader for Cook-Torrance, with texturing.
It consider a single directional light.

author: Davide Gadia

Real-time Graphics Programming - a.a. 2017/2018
Master degree in Computer Science
Universita' degli Studi di Milano

*/

#version 330 core

// output shader variable
out vec4 colorFrag;

// light incidence direction (calculated in vertex shader, interpolated by rasterization)
in vec3 lightDir;
// the transformed normal has been calculated per-vertex in the vertex shader
in vec3 vNormal;
// vector from fragment to camera (in view coordinate)
in vec3 vViewPosition;

// model matrix
uniform mat4 modelMatrix;
// view matrix
uniform mat4 viewMatrix;

// vector from fragment to camera (in view coordinate)
in vec2 interp_UV;

//variables for wet effect
in vec4 mvPosition;
in vec3 worldPos;
in vec3 worldNormal;
in vec4 localVertexPosition;

// texture repetitions
uniform float repeat;

// texture sampler
uniform sampler2D tex;

//fog variables
uniform int fogActive;
uniform vec3 eyePosition;

//particles and textures variables
uniform vec4 particleColor; //color of the particle to render
uniform int hasTexture;     //if true, output color is particleColor


// fog variables
const vec3 DiffuseLight = vec3(0.15, 0.05, 0.0);
const vec3 RimColor  = vec3(0.2, 0.2, 0.2);
const vec3 fogColor = vec3(0.5,0.5,0.5);
in float distVertex;

//snow effect constants
uniform vec3 snowDirection;
uniform float snowLevel;    //range between [-1, 1]
float snowMixValue = 0.2;

//all credits goes to: https://github.com/hughsk/glsl-hemisphere-light
vec3 hemisphere_light(vec3 normal, vec3 sky, vec3 ground,
        vec3 lightDirection, mat4 modelMatrix, mat4 viewMatrix, vec3 viewPosition) {
  vec3 direction = normalize((
    modelMatrix * vec4(lightDirection, 1.0)
  ).xyz);

  float weight = 0.5 * dot(
      normal
    , direction
  ) + 0.5;

  return mix(ground, sky, weight);
}


void main()
{
    //hasTexture == 0 => it's a particle
    vec4 surfaceColor;
    float alpha;
    //check if i need to draw snow on the map
    bool enoughSnow = dot(normalize(worldNormal), snowDirection) >= snowLevel;
    //hasTexture == 0 => rendering particle
    if (hasTexture == 0) {
        surfaceColor = particleColor;
        alpha = surfaceColor.a;
    }
    //hasTexture == 1 => map, get data from texture
    else{
        //enoughSnow => render snow effect
        vec2 repeated_Uv = mod(interp_UV*repeat, 1.0);
        vec4 textureColor = texture(tex, repeated_Uv);
        if(enoughSnow){
            surfaceColor = mix(particleColor, textureColor, snowMixValue);
            alpha = 1.0;
        }else{
            //!enoughSnow => render texture
            surfaceColor = textureColor;
            alpha = 1.0;
        }
    }
    //illuminatedColor is rgb component of final color, alpha only the a channel
    vec3 illuminatedColor;
    if(hasTexture == 0 ){
        //for the particle (and low level snow), we use hemisphere lighting
        illuminatedColor = hemisphere_light(vNormal, surfaceColor.xyz, surfaceColor.xyz, lightDir, modelMatrix, viewMatrix, vViewPosition);
    }else{
        //for texture, we use snow effect
        illuminatedColor = surfaceColor.rgb;
    }
    //finally set output color variable
    colorFrag = vec4(illuminatedColor, alpha);
    //if fog is enabled, we draw fog (not working with snow)
    if (fogActive == 1) {
        vec3 texColor = colorFrag.rgb;

        //get light an view directions
        vec3 Lfog = normalize(lightDir - worldPos);
        vec3 Vfog = normalize(eyePosition - worldPos);

        //diffuse lighting
        vec3 diffuse = DiffuseLight * max(0, dot(Lfog, worldNormal));
        
        //rim lighting
        float rim = 1 - max(dot(Vfog, worldNormal), 0.0);
        rim = smoothstep(0.6, 1.0, rim);
        vec3 finalRim = RimColor * vec3(rim, rim, rim);


        //get all lights and texture
        vec3 finalColor = finalRim + diffuse + texColor;

        vec3 c = vec3(0,0,0);

        float be = 0.025 * smoothstep(0.0, 6.0, 10.0 - mvPosition.y);
        float bi = 0.035 * smoothstep(0.0, 80, 10.0 - mvPosition.y);
        float ext =  exp(-distVertex * be);
        float insc = exp(-distVertex * bi);

        c = finalColor * ext + fogColor * (1 - insc);

        colorFrag = vec4(c, 1);
    }
}
