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

in vec4 mvPosition;
in float distVertex;
in vec3 worldPos;
in vec3 worldNormal;

// texture repetitions
uniform float repeat;

// texture sampler
uniform sampler2D tex;

// check fog
uniform int fogActive;
uniform vec3 eyePosition;

uniform vec4 particleColor; //color of the particle to render
uniform int hasTexture;     //if true, output color is particleColor

vec4 skyColor = particleColor;  //color of the sky for hemisphere lighting

// fog variables
const vec3 DiffuseLight = vec3(0.15, 0.05, 0.0);
const vec3 RimColor  = vec3(0.2, 0.2, 0.2);
const vec3 fogColor = vec3(0.5,0.5,0.5);

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
    vec4 surfaceColor;
    if (hasTexture == 0) {
        surfaceColor = particleColor;
    }
    else{
        vec2 repeated_Uv = mod(interp_UV*repeat, 1.0);
        surfaceColor = texture(tex, repeated_Uv);
    }
    vec3 illuminatedColor = hemisphere_light(vNormal, skyColor.xyz, surfaceColor.xyz, lightDir, modelMatrix, viewMatrix, vViewPosition);
    colorFrag = vec4(illuminatedColor, 1.0);
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
