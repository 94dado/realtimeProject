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

const vec4 skyColor = vec4(122/255.0, 162/255.0, 226/255.0, 1.0);  //color of the sky for hemisphere lighting

// fog variables
const vec3 DiffuseLight = vec3(0.15, 0.05, 0.0);
const vec3 RimColor  = vec3(0.2, 0.2, 0.2);
const vec3 fogColor = vec3(0.5,0.5,0.5);

//wet effect variables
out vec4 localVertexPosition;

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

float basicNoise(vec2 co){
    return 0.5 + 0.5 * fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

//3D Value Noise generator by Morgan McGuire @morgan3d
//https://www.shadertoy.com/view/4dS3Wd
float hash(float n) { return fract(sin(n) * 1e4); }
float hash(vec2 p) { return fract(1e4 * sin(17.0 * p.x + p.y * 0.1) * (0.1 + abs(sin(p.y * 13.0 + p.x)))); }

float noise(vec3 x) {
	const vec3 step = vec3(110, 241, 171);

	vec3 i = floor(x);
	vec3 f = fract(x);

	// For performance, compute the base input to a 1D hash from the integer part of the argument and the
	// incremental change to the 1D based on the 3D -> 1D wrapping
    float n = dot(i, step);

	vec3 u = f * f * (3.0 - 2.0 * f);
	return mix(mix(mix( hash(n + dot(step, vec3(0, 0, 0))), hash(n + dot(step, vec3(1, 0, 0))), u.x),
		   mix( hash(n + dot(step, vec3(0, 1, 0))), hash(n + dot(step, vec3(1, 1, 0))), u.x), u.y),
	       mix(mix( hash(n + dot(step, vec3(0, 0, 1))), hash(n + dot(step, vec3(1, 0, 1))), u.x),
		   mix( hash(n + dot(step, vec3(0, 1, 1))), hash(n + dot(step, vec3(1, 1, 1))), u.x), u.y), u.z);
}

//Fractional Brownian Motion
#define NUM_OCTAVES 2

float fnoise(vec3 x) {
	float v = 0.0;
	float a = 0.5;
	vec3 shift = vec3(100);
	for (int i = 0; i < NUM_OCTAVES; ++i) {
		v += a * noise(x);
		x = x * 2.0 + shift;
		a *= 0.5;
	}
	return v;
}

//wet_effect(surfaceColor)
vec3 wet_effect(vec3 materialDiffuseColor){
    vec3 color;
    vec3 lightColor = vec3(1, 1, 1);
    float lightPower = 50.0;

    vec3 materialAmbientColor = vec3(0.1, 0.1, 0.1) * materialDiffuseColor;
    vec3 materialSpecularColor = vec3(0.3, 0.3, 0.3);

    float distance = distVertex;

    vec3 n = normalize(vNormal);
    vec3 l = normalize(lightDir);

    //Generating a bump map from noise
    float inz = fnoise(localVertexPosition.xyz) * 0.5 + 0.5;
    float E = 0.001;

    vec3 px = localVertexPosition.xyz;
    px.x += E;
    vec3 py = localVertexPosition.xyz;
    py.y += E;
    vec3 pz = localVertexPosition.xyz;
    pz.z += E;

    vec3 bump = vec3(fnoise(px)*0.5+0.5, fnoise(py)*0.5+0.5, fnoise(pz)*0.5+0.5);
    vec3 pN = vec3((bump.x-inz)/E, (bump.y-inz)/E, (bump.z-inz)/E);

    n = normalize(n - pN);

    float cosTheta = clamp(dot(n, l), 0, 1);

    vec3 e = normalize(vViewPosition);
    vec3 r = reflect(-l, n);

    float cosAlpha = clamp(dot(e,r), 0, 1);

    float F0 = 0.5;
    vec3 h = normalize(e+l);
    float base = 1 - dot(e, h);
    float exponential = pow(base, 5.0);
    float fresnel = exponential + F0 * (1.0 - exponential);


    color = materialAmbientColor
	    + 0.5 * (materialDiffuseColor * lightColor) * lightPower * cosTheta / (distance * distance)
	    + ((materialSpecularColor * lightColor) * basicNoise(localVertexPosition.xy) * 20
		* lightPower * pow(max(0.0, cosAlpha), 200) / (distance * distance))
		* fresnel;

    color = clamp(color, 0., 1.);
    return color;
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
    //illuminatedColor = wet_effect(surfaceColor.xyz);
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
