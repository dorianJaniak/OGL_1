#pragma once

namespace dj {

const char* const particleSystem_vsSource = R"(
#version 330 core
layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec2 a_texCoord;
layout (location = 2) in mat4 a_model;

uniform mat4 u_scale;
uniform mat4 u_camVP;

out vec2 v_texCoords;

void main()
{
	v_texCoords = a_texCoord;
	gl_Position = u_camVP * a_model * u_scale * vec4(a_pos, 1.0f);
}
)";

const char* const particleSystem_fsSource = R"(
#version 330 core

in vec2 v_texCoords;
out vec4 FragColor;

uniform sampler2D u_texture;
uniform float u_opacity;

void main()
{
	vec4 texColor = texture(u_texture, v_texCoords);
	if (texColor.a < 0.05f)
	{
		discard;
	}
	texColor.a *= u_opacity;
	FragColor = texColor;
}
)";

const char* const debugBoundingBox_vsSource = R"(
#version 330 core


)";

const char* const debugMesh_vsSource = R"(
#version 330 core
layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_norm;
layout (location = 2) in vec3 a_tang;
layout (location = 3) in vec3 a_bitang;
uniform mat4 u_model;
uniform mat4 u_view;
uniform mat3 u_normalMat;

out OUT_VS {
	vec3 normal;
	vec3 tang;
	vec3 bitang;
} out_vs;

void main()
{
	gl_Position =  u_view * u_model * vec4(a_pos, 1.0f);
	out_vs.normal = normalize(vec3(vec4(u_normalMat * a_norm, 0.0f)));
	out_vs.tang = normalize(vec3(vec4(u_normalMat * a_tang, 0.0f)));
	out_vs.bitang = normalize(vec3(vec4(u_normalMat * a_bitang, 0.0f)));
}
)";

const char* const debugMesh_gsSource = R"(
#version 330 core

layout (triangles) in;
layout (line_strip, max_vertices = 18) out; 

uniform mat4 u_proj;
uniform float u_axisLength;
uniform uint u_debugSelection;

in OUT_VS {
	vec3 normal;
	vec3 tang;
	vec3 bitang;
} out_vs[];

out vec4 v_color;

void drawNormal(uint index)
{
	v_color = vec4(1.0f, 1.0f, 0.5f, 1.0f);
	gl_Position = u_proj * gl_in[index].gl_Position;
	EmitVertex();

	gl_Position = u_proj * (gl_in[index].gl_Position + vec4(out_vs[index].normal, 0.0f) * u_axisLength);
	EmitVertex();
	EndPrimitive();
}

void drawTang(uint index)
{
	v_color = vec4(0.5f, 1.0f, 1.0f, 1.0f);
	gl_Position = u_proj * gl_in[index].gl_Position;
	EmitVertex();

	gl_Position = u_proj * (gl_in[index].gl_Position + vec4(out_vs[index].tang, 0.0f) * u_axisLength);
	EmitVertex();
	EndPrimitive();
}

void drawBitang(uint index)
{
	v_color = vec4(1.0f, 0.5f, 1.0f, 1.0f);
	gl_Position = u_proj * gl_in[index].gl_Position;
	EmitVertex();

	gl_Position = u_proj * (gl_in[index].gl_Position + vec4(out_vs[index].bitang, 0.0f) * u_axisLength);
	EmitVertex();
	EndPrimitive();
}

void main()
{
	if (u_debugSelection == 1u)
	{
		drawNormal(0u);
		drawNormal(1u);
		drawNormal(2u);
	} 
	else if (u_debugSelection == 2u)
	{
		drawTang(0u);
		drawTang(1u);
		drawTang(2u);
	}
	else if (u_debugSelection == 3u)
	{
		drawBitang(0u);
		drawBitang(1u);
		drawBitang(2u);
	}
}
)";

const char* const debugMesh_fsSource = R"(
#version 330 core

in vec4 v_color;
out vec4 FragColor;

void main()
{
	FragColor = v_color;
}
)";

const char* const skybox_vsSource = R"(
#version 330 core
layout (location = 0) in vec3 a_pos;
uniform mat4 u_camRP;

out vec3 v_texCoords;

void main()
{
	v_texCoords = a_pos;
	vec4 pos = u_camRP * vec4(a_pos, 1.0f);
	gl_Position = pos.xyww;
	//gl_Position = pos;
}
)";

const char* const skybox_fsSource = R"(
#version 330 core

uniform samplerCube u_skybox;

in vec3 v_texCoords;
out vec4 FragColor;

void main()
{
	FragColor = texture(u_skybox, v_texCoords);
}
)";

const char* const depthTestCube_vsSource = R"(
#version 330 core
layout (location = 0) in vec3 a_pos;
uniform mat4 u_model;

void main()
{
	gl_Position = u_model * vec4(a_pos, 1.0f);
}
)";

const char* const depthTestCube_gsSource = R"(
#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices=18) out;

uniform mat4 u_cameraMatrices[6];

out vec4 v_fragPos;
uniform float u_far;

void main()
{
	for (int face = 0; face < 6; ++face)
	{
		gl_Layer = face;
		for (uint i = 0u; i < 3u; ++i)
		{
			v_fragPos = gl_in[i].gl_Position;
			gl_Position = u_cameraMatrices[face] * v_fragPos;
			EmitVertex();
		}
		EndPrimitive();
	}
}
)";

const char* const depthTestCube_fsSource = R"(
#version 330 core

in vec4 v_fragPos;

uniform vec3 u_lightPos;
uniform float u_far;

void main()
{
	float lightDistance = length(v_fragPos.xyz - u_lightPos);

	lightDistance /= u_far;
	gl_FragDepth = lightDistance;
}
)";

const char* const depthTestMat_vsSource = R"(
#version 330 core
layout (location = 0) in vec3 a_pos;
uniform mat4 u_camVP;
uniform mat4 u_model;

void main()
{
	vec4 pos = u_camVP * u_model * vec4(a_pos, 1.0f);
	gl_Position = pos;
}
)";

const char* const depthTestMat_fsSource = R"(
#version 330 core

uniform float u_near;
uniform float u_far;

float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // Back to NDC 
    return (2.0 * u_near * u_far) / (u_far + u_near - z * (u_far - u_near));
}

void main()
{
	gl_FragDepth = LinearizeDepth(gl_FragCoord.z) / u_far;
}
)";

const char* const fullscreenFBO_vsSource = R"(
#version 330 core
layout (location = 0) in vec2 a_pos;
layout (location = 1) in vec2 a_texCoord;

out vec2 v_texCoord;

void main()
{
	gl_Position = vec4(a_pos.x, a_pos.y, 0.0f, 1.0f);
	v_texCoord = a_texCoord;
}
)";

const char* const fullscreenFBO_fsSource = R"(
#version 330 core

uniform sampler2D frame;

in vec2 v_texCoord;

//const float kernel [9] = {
//	1.0f / 16.0f,	2.0f / 16.0f,	1.0f / 16.0f,
//	2.0f / 16.0f,	4.0f / 16.0f,	2.0f / 16.0f,
//	1.0f / 16.0f,	2.0f / 16.0f,	1.0f / 16.0f,
//};

const float kernel [9] = float[](
	-1.0f,	0.0f,	-1.0f,
	-1.0f,	7.0f,	-1.0f,
	-1.0f,	0.0f,	-1.0f
);

const float offset = 1.0f / 500.0f;	// 10.0f

void main()
{
	vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // top-left
        vec2( 0.0f,    offset), // top-center
        vec2( offset,  offset), // top-right
        vec2(-offset,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( offset,  0.0f),   // center-right
        vec2(-offset, -offset), // bottom-left
        vec2( 0.0f,   -offset), // bottom-center
        vec2( offset, -offset)  // bottom-right    
    );

	vec3 sampleTex[9];
	for (int i = 0; i < 9; ++i)
	{
		sampleTex[i] = texture(frame, v_texCoord + offsets[i]).rgb;
	}

	vec3 color = vec3(0.0f);
	for (int i = 0; i < 9; ++i)
	{
		color += sampleTex[i] * kernel[i];
	}

	//gl_FragColor = vec4(color, 1.0f);
	//gl_FragColor = vec4(depth, depth, depth, 1.0f);
	gl_FragColor = texture(frame, v_texCoord);
}
)";


const char* const prePBR_vsSource = R"(
#version 330 core
layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_norm;
layout (location = 2) in vec3 a_tang;
layout (location = 3) in vec3 a_bitang;
layout (location = 4) in vec2 a_texCoord;
uniform mat4 u_mvp;
uniform mat4 u_model;
uniform mat4 u_lightVP[2];
uniform mat3 u_normalMat;

out vec3 v_wPos;
out vec3 v_wNorm;
out vec2 v_texCoord;
out mat3 v_TBN;
out vec4 v_fragLightSpacePos[2];
out float v_z;

void main()
{
	vec3 T = normalize(vec3(u_model * vec4(a_tang, 0.0f)));
	vec3 N = normalize(vec3(u_model * vec4(a_norm, 0.0f)));
	vec3 B = normalize(vec3(u_model * vec4(a_bitang, 0.0f)));

	v_TBN = mat3(T, B, N);

	vec4 pos = u_mvp * vec4(a_pos, 1.0f);
	v_wPos = vec3(u_model * vec4(a_pos, 1.0f));
	v_fragLightSpacePos[0] = u_lightVP[0] * vec4(v_wPos, 1.0f);
	v_fragLightSpacePos[1] = u_lightVP[1] * vec4(v_wPos, 1.0f);

	gl_Position = pos;
	v_texCoord = a_texCoord;
	v_wNorm = u_normalMat * a_norm;
	v_z = 1.0f - normalize(pos).z;
}
)";

const char* const pbr_fsSource = R"(
#version 330 core
#define MAX_LIGHTS 4u

#define LIGHT_TYPE_POINT 1u
#define LIGHT_TYPE_SPOT 2u

struct PBRMaterial {
	sampler2D albedo;
	sampler2D roughness;
	sampler2D normal;
	sampler2D metallic;
};

struct Light {
	// General parameters
	vec3 pos;
	vec3 color;
	float range;
	uint type;
	
	// Direction parameters
	vec3 dir;

	// Spot parameters
	float spotExtCos;
	float spotIntCos;

	// Shadow parameters
	bool shadowActive;
};

uniform PBRMaterial u_material;
uniform sampler2D u_shadow[2];
uniform samplerCube u_cubeShadow[2];
uniform samplerCube u_skybox;
uniform Light u_light[MAX_LIGHTS];
uniform uint u_lightsCount;
uniform vec3 u_wCamPos;
uniform float u_lightNear;
uniform float u_lightFar;

out vec4 FragColor;

in vec3 v_wPos;
in vec3 v_wNorm;
in vec2 v_texCoord;
in mat3 v_TBN;
in vec4 v_fragLightSpacePos[2];
in float v_z;


float shadowSoft = 0.5f;
float PI = 3.1415926f;

struct ColorOutput {
	vec3 diffuse;
	vec3 specular;
};

float linearizeDepth(in float depth)
{
    float z = depth * 2.0 - 1.0; // Back to NDC 
    return (2.0 * u_lightNear * u_lightFar) / (u_lightFar + u_lightNear - z * (u_lightFar - u_lightNear));
}

float calculateShadowsPoint(in vec3 lightPos, in vec3 lightDir, in vec3 lightDirN, in vec3 fragPos, uint lightNo)
{
	//vec3 lightDir = fragPos - lightPos;
	//vec3 lightDirN = normalize(lightDir);
	//vec3 lightDir2N = normalize(lightPos - fragPos);
	float lightDist = length(lightDir);
	
	float shadowDist = texture(u_cubeShadow[lightNo], lightDir).r * 25.0f;
	
	//float bias = 0.05f;
	//float bias = clamp(0.3f * (1.0f - dot(v_wNorm, -lightDirN)), 0.001f, 0.3f);
	//float bias = clamp(0.05f * (1.0f - dot(v_wNorm, lightDirN)), 0.0001f, 0.001f);
	float bias = 0.005f;
	float lightDistBiased = lightDist - bias;
	float shadow = lightDist - bias > shadowDist ? 1.0f : 0.0f;

	const uint maxSamplesPerSide = 4u;		// 4u
	const float halfMaxSamples = maxSamplesPerSide / 2.0f;

	float shFuzz = (lightDist - shadowDist) * shadowSoft * 0.1f / maxSamplesPerSide;
	uint shadowSamples = 0u;

	if (shadow > 0.0f)
	{
		vec3 sampleStart = lightDir - vec3(shFuzz * halfMaxSamples);

		for (uint i = 0u; i < maxSamplesPerSide; i++)
		{
			for (uint j = 0u; j < maxSamplesPerSide; j++)
			{
				for (uint k = 0u; k < maxSamplesPerSide; k++)
				{
					vec3 sampleCoords = sampleStart + shFuzz * vec3(i, j, k); 

					float shDepth = texture(u_cubeShadow[lightNo], sampleCoords).r * 25.0f;
					shadowSamples += (lightDistBiased > shDepth) ? 1u : 0u;
				}
			}
		}
	}

	return pow(float(shadowSamples) / (maxSamplesPerSide * maxSamplesPerSide * maxSamplesPerSide), 3u);
}

// 1.0f - shadowed
// 0.0f - light
float calculateShadows(in vec3 normalN, in vec3 lightDirN, in uint lightNo)
{
	float factor = 0.0f;
	// Transform to [-1; 1]
	vec3 projCoords = v_fragLightSpacePos[lightNo].xyz / v_fragLightSpacePos[lightNo].w;

	// Transform to UV coords ([0; 1])
	projCoords = projCoords * 0.5 + 0.5;

	// Both from the perspective of light
	float shadowDepth = texture(u_shadow[lightNo], projCoords.xy).r;
	float fragDepth = linearizeDepth(projCoords.z) / u_lightFar;

	//float bias = min(0.002f * (1.0 - dot(normalN, lightDirN)), 0.005f);
	float bias = clamp(0.05f * (1.0f - dot(v_wNorm, lightDirN)), 0.0001f, 0.001f);
	float fragDepthBiased = fragDepth - bias;
	float shadow = (fragDepth - bias > shadowDepth) ? 1.0f : 0.0f;

	const uint maxSamples = 7u;	// 7u
	const float halfMaxSamples = maxSamples / 2u;

	float shFuzz = (fragDepth - shadowDepth) * shadowSoft / maxSamples;

	uint shadowSamples = 0u;

	if (shadow > 0.0f)
	{
		for (uint i = 0u; i < maxSamples; ++i)
		{
			float xCoord = projCoords.x - shFuzz * (halfMaxSamples - i);
			for (uint j = 0u; j < maxSamples; ++j)
			{
				float yCoord = projCoords.y - shFuzz * (halfMaxSamples - j);
				float shDepth = texture(u_shadow[lightNo], vec2(xCoord, yCoord)).r;
				shadowSamples += (fragDepthBiased > shDepth) ? 1u : 0u;
			}
		}
	}

	return pow(float(shadowSamples) / (maxSamples * maxSamples), 3);
}

vec3 applyNormalMap()
{
	vec3 normal = texture(u_material.normal, v_texCoord).rgb;
	normal = normal * 2.0f - 1.0f;
	return normalize(v_TBN * normal);
}

// albedo - albedo color from texture
// metallic - metallic factor from texture
vec3 calcF0ForFresnel(in vec3 albedo, in float metallic)
{
	vec3 f0 = vec3(0.04f);
	return mix(f0, albedo, metallic);
}

// r - roughness
// cosHN - positive cosinus of halfway vector and normal vector (both normalized)
float calcDistrib_CookTorrance(in float r, in float cosHN)
{
	float r2 = r * r;
	float denom = (cosHN * cosHN * (r2 - 1) + 1);
	return r2 / (PI * denom * denom);
}

// f0 - surface reflection at zero incidence (reflection when view vector is aligned with normal vector)
// cosHN - positive cosinus of halfway vector and normal vector (both normalized)
vec3 calcFresnel(in vec3 f0, in float cosHN)
{
	return f0 + (1.0f - f0) * pow(clamp(1.0f - cosHN, 0, 1.0), 5);
}

// cosAN - positive cosinus of normal vector and one of these: light vector OR view vector
// k - coefficient
float calcGeometryShadowing_Schlick(in float cosAN, in float k)
{
	float denom = cosAN * (1.0f - k) + k;
	return (cosAN / denom);
}

float calcGeometryShadowing_Schlick2(in float cosVN, in float cosLN, in float roughn)
{
	float k = (roughn + 1.0f) * (roughn + 1.0f) / 8.0f;
	return calcGeometryShadowing_Schlick(cosVN, k) * calcGeometryShadowing_Schlick(cosLN, k);
}

float calcSpotlightIntensity(in Light light, in vec3 lightRayDirN)
{
	//float spotFactor = 1.0f;

	//if (light.type == LIGHT_TYPE_SPOT)
	//{
		float spotCos = dot(normalize(light.dir), lightRayDirN);
		float cutOffRange = light.spotIntCos - light.spotExtCos;
		float spotFactor = clamp((spotCos - light.spotExtCos) / cutOffRange, 0.0f, 1.0f);	

		// power of 2 to make soft lights softer, and hard lights harder
		spotFactor *= spotFactor;
	//}

	return spotFactor;
}

// normN - normal normalized
// viewN - view direction normalized (from camera to fragment)
// r - roughness
// fresnelF0 - f0 for fresnel
ColorOutput calcLights(in vec3 normN, in vec3 viewN, in float roughn, in float metallic, in vec3 fresnelF0)
{
	ColorOutput co;
	co.diffuse = vec3(0.0f);
	co.specular = vec3(0.0f);

	// Calculate angles and distances related to camera
	float cosVN = max(dot(normN, viewN), 0.0f);
	uint spotIndex = 0u;
	for (uint i = uint(0); i < u_lightsCount; ++i)
	{
		// Initial values
		ColorOutput coPart;
		float negShadowFactor = 1.0f;
		float inSpot = 1.0f;
		bool isSpot = (u_light[i].type == LIGHT_TYPE_SPOT);

		// Calculate light related vectors
		vec3 lightDir = u_light[i].pos - v_wPos;
		vec3 lightN = normalize(lightDir);
		vec3 halfN = normalize(lightN + viewN);
		vec3 reflectedLightN = reflect(-lightN, normN);

		// Compute shadow and spotlight factors
		if (u_light[i].shadowActive)
		{
			negShadowFactor = 1.0f - (isSpot ? calculateShadows(normN, lightN, spotIndex)
											 : calculateShadowsPoint(u_light[i].pos, -lightDir, -lightN, v_wPos, i - spotIndex));
		}
		spotIndex += (isSpot ? 1u : 0u);
		inSpot = (isSpot ? calcSpotlightIntensity(u_light[i], -lightN) : 1.0f);

		// For fragments in spotlights and in point lights compute diffuse and specular
		//if (inSpot > 0.005f)
		//{
			// Calculate angles and distances related to light
			float lightDist = length(lightDir);
			float atten = 1.0f / (lightDist * lightDist);
			vec3 radiance = atten * u_light[i].color;
			float cosLN = max(dot(normN, lightN), 0.0f);
			float cosHN = max(dot(normN, halfN), 0.0f);

			// Calculate BRDF
			float D = calcDistrib_CookTorrance(roughn, cosHN);
			vec3 F = calcFresnel(fresnelF0, cosHN);
			float G = calcGeometryShadowing_Schlick2(cosVN, cosLN, roughn);

			vec3 nominator = D * G * F;
			float denominator = 4.0f * cosVN * cosLN + 0.0001f;
			vec3 specular = nominator / denominator;

			vec3 kD = vec3(1.0f) - F;
			kD *= (1.0f - metallic);
		

			coPart.specular = negShadowFactor * inSpot * specular * radiance * cosLN;
			coPart.diffuse = negShadowFactor * inSpot * kD * radiance * cosLN / PI;

			co.diffuse += coPart.diffuse;
			co.specular += coPart.specular;
		//}
	}

	return co;
}

void main()
{
	//vec3 normalN = normalize(v_wNorm);
	vec3 normalN = applyNormalMap();
	vec3 viewDirN = normalize(u_wCamPos - v_wPos);
	vec3 viewReflDirN = reflect(-viewDirN, normalN);

	//vec4 textureMix = 0.5f * texture(u_texture1, v_texCoord) + 0.5f * texture(u_texture2, v_texCoord);
	vec4 textureAlbedo = texture(u_material.albedo, v_texCoord);
	float textureRoughness = texture(u_material.roughness, v_texCoord).r;
	float textureMetallic = texture(u_material.metallic, v_texCoord).r;
	vec4 textureSkybox = texture(u_skybox, viewReflDirN);
	//vec3 ambientCol = 0.2f * v_z * textureAlbedo.rgb;

	vec3 fresnelF0 = calcF0ForFresnel(textureAlbedo.rgb, textureMetallic);

	ColorOutput co = calcLights(normalN, viewDirN, textureRoughness, textureMetallic, fresnelF0);
	co.diffuse *= textureAlbedo.rgb;
	co.specular += (textureSkybox.rgb * textureMetallic) * (1.0f - textureRoughness);

	vec3 color = co.diffuse + co.specular; //+ ambientCol;
	FragColor = vec4(color, 1.0f);
}

)";

const char* const prePBR_fsSource = R"(
#version 330 core

#define MAX_LIGHTS 4u

#define LIGHT_TYPE_POINT 1u
#define LIGHT_TYPE_SPOT 2u

struct PBRMaterial {
	sampler2D albedo;
	sampler2D roughness;
	sampler2D normal;
};

struct Light {
	// General parameters
	vec3 pos;
	vec3 color;
	float range;
	uint type;
	
	// Direction parameters
	vec3 dir;

	// Spot parameters
	float spotExtCos;
	float spotIntCos;

	// Shadow parameters
	uint shadowActive;
};

struct LightAttenuation {
	float constant;
	float linear;
	float quadratic;
};

uniform PBRMaterial u_material;
uniform sampler2D u_shadow[2];
uniform samplerCube u_skybox;
//uniform vec3 u_color;
uniform Light u_light[MAX_LIGHTS];
uniform uint u_lightsCount;
uniform sampler2D u_texture1;
uniform sampler2D u_texture2;
uniform vec3 u_wCamPos;
uniform float u_lightNear;
uniform float u_lightFar;

out vec4 FragColor;

in vec3 v_wPos;
in vec3 v_wNorm;
in vec2 v_texCoord;
in mat3 v_TBN;
in vec4 v_fragLightSpacePos[2];
in float v_z;

struct ColorOutput {
	vec3 diffuse;
	vec3 specular;
};

float linearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // Back to NDC 
    return (2.0 * u_lightNear * u_lightFar) / (u_lightFar + u_lightNear - z * (u_lightFar - u_lightNear));
}

float shadowSoft = 0.5f;

// 1.0f - shadowed
// 0.0f - light
float calculateShadows(in vec3 normalN, in vec3 lightDirN, in uint lightNo)
{
	float factor = 0.0f;
	// Transform to [-1; 1]
	vec3 projCoords = v_fragLightSpacePos[lightNo].xyz / v_fragLightSpacePos[lightNo].w;

	// Transform to UV coords ([0; 1])
	projCoords = projCoords * 0.5 + 0.5;

	// Both from the perspective of light
	float shadowDepth = texture(u_shadow[lightNo], projCoords.xy).r;
	float fragDepth = linearizeDepth(projCoords.z) / u_lightFar;

	//float bias = min(0.002f * (1.0 - dot(normalN, lightDirN)), 0.005f);
	float bias = clamp(0.05f * (1.0f - dot(v_wNorm, lightDirN)), 0.0001f, 0.001f);
	float shadow = (fragDepth - bias > shadowDepth) ? 1.0f : 0.0f;

	const uint maxSamples = 7u;
	const float halfMaxSamples = maxSamples / 2u;

	float shFuzz = (fragDepth - shadowDepth) * shadowSoft / maxSamples;

	float shadowPoints = 0u;

	if (shadow > 0.0f) 
	{
		for (uint i = 0u; i < maxSamples; ++i)
		{
			float xCoord = projCoords.x - shFuzz * (halfMaxSamples - i);
			for (uint j = 0u; j < maxSamples; ++j)
			{
				float yCoord = projCoords.y - shFuzz * (halfMaxSamples - j);
				float shDepth = texture(u_shadow[lightNo], vec2(xCoord, yCoord)).r;
				shadowPoints += (fragDepth - bias > shDepth) ? 1.0f : 0.0f;
			}
		}
	}

	return pow(shadowPoints / (maxSamples * maxSamples), 3);
}

float calcSpotlightIntensity(in Light light, in vec3 lightRayDirN)
{
	float spotFactor = 1.0f;

	if (light.type == LIGHT_TYPE_SPOT)
	{
		float spotCos = dot(normalize(light.dir), -lightRayDirN);
		float cutOffRange = light.spotIntCos - light.spotExtCos;
		
		spotFactor = clamp((spotCos - light.spotExtCos) / cutOffRange, 0.0f, 1.0f);
	}

	return spotFactor;
}

// Computes Diffuse and Specular Colors
ColorOutput calcLight(in Light light, in LightAttenuation atten, in vec3 normN, in vec3 viewDirN)
{
	// Vectors preparation
	vec3 lightDir = light.pos - v_wPos;
	vec3 lightDirN = normalize(lightDir);
	vec3 halfwayDirN = normalize(lightDirN + viewDirN);
	vec3 reflectedLightDir = reflect(-lightDirN, normN);
	
	// Vectors computations
	float distance = length(lightDir);
	float diffuseDot = max(dot(normN, lightDirN), 0.0f);
	float specularDot = max(dot(normN, halfwayDirN), 0.0f);
	//float specularDot = max(dot(viewDirN, reflectedLightDir), 0.0f);

	// Factors computations
	float attenuation = 1.0f / (atten.constant + atten.linear * distance + atten.quadratic * distance * distance);
	float rangeFactor = 1.0f - smoothstep(light.range * 0.5f, light.range, distance);
	float specularFactor = pow(specularDot, 128);	
	float spotFactor = calcSpotlightIntensity(light, lightDirN);

	// Factors adjustment
	diffuseDot = pow(diffuseDot, 0.5f);

	// Final colors computations
	float diffuseIntensity = spotFactor * rangeFactor * attenuation * diffuseDot;
	float specularIntensity = spotFactor * attenuation * specularFactor;

	ColorOutput co;
	co.diffuse = diffuseIntensity * light.color;
	co.specular = specularIntensity * light.color;
	return co;
}

ColorOutput calcLights(in vec3 normN, in vec3 viewDirN)
{
	// Setting Light attenuation (for all components)
	//LightAttenuation atten (1.0f, 0.09f, 0.032f);
	LightAttenuation atten;
	atten.constant = 1.0f;
	atten.linear = 0.09f;
	atten.quadratic = 0.032f;

	ColorOutput co;
	co.diffuse = vec3(0.0f, 0.0f, 0.0f);
	co.specular = vec3(0.0f, 0.0f, 0.0f);
	for (uint i = uint(0); i < MAX_LIGHTS; ++i)
	{
		ColorOutput coPart = calcLight(u_light[i], atten, normN, viewDirN);

		if (u_light[i].shadowActive) 
		{
			vec3 lightDir = u_light[i].pos - v_wPos;
			vec3 lightDirN = normalize(lightDir);
			float negShadowFactor = 1.0f - calculateShadows(normN, lightDirN, i - 2u);
			coPart.diffuse *= negShadowFactor;
			coPart.specular *= negShadowFactor;
		}

		co.diffuse += coPart.diffuse;
		co.specular += coPart.specular;
	}

	return co;
}

void main()
{
	//vec3 normalN = normalize(v_wNorm);
	vec3 normal = texture(u_material.normal, v_texCoord).rgb;
	normal = normal * 2.0f - 1.0f;
	vec3 normalN = normalize(v_TBN * normal);
	vec3 viewDirN = normalize(u_wCamPos - v_wPos);
	vec3 viewReflDirN = reflect(-viewDirN, normalN);

	//vec4 textureMix = 0.5f * texture(u_texture1, v_texCoord) + 0.5f * texture(u_texture2, v_texCoord);
	vec4 textureAlbedo = texture(u_material.albedo, v_texCoord);
	vec3 textureSpecular = vec3(1.0f) - texture(u_material.roughness, v_texCoord).rgb;
	vec4 textureSkybox = texture(u_skybox, viewReflDirN);
	vec3 ambientCol = 0.2f * v_z * textureAlbedo.rgb;

	ColorOutput co = calcLights(normalN, viewDirN);
	co.diffuse *= textureAlbedo.rgb;
	co.specular *= textureSpecular;
	co.specular += textureSkybox.rgb * textureSpecular.rgb;

	vec3 color = co.diffuse + co.specular; //+ ambientCol;
	FragColor = vec4(color, 1.0f);
}
)";

} // namespace dj