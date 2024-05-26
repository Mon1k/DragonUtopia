#include "shadow_common.hlsl"

cbuffer LightBuffer
{
    float4 m_ambientColor;
    float4 m_diffuseColor;
    float3 m_lightDirection;
    float m_lightIntensity;
    float m_isSoftShadow;
    float m_isDirection;
    float m_ShadowSize;
    float m_padding;
};

cbuffer MatrixBuffer
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
    matrix lightViewMatrix;
    matrix lightProjectionMatrix;
};

cbuffer LightBuffer2
{
    float3 lightPosition;
    float padding;
};


struct VertexInputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float4 lightViewPosition : TEXCOORD1;
    float3 lightPos : TEXCOORD2;
};

PixelInputType ShadowVertexShader(VertexInputType input)
{
    PixelInputType output;
    float4 worldPosition;
    
    // Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

    // Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

    // Calculate the position of the vertice as viewed by the light source.
    output.lightViewPosition = mul(input.position, worldMatrix);
    output.lightViewPosition = mul(output.lightViewPosition, lightViewMatrix);
    output.lightViewPosition = mul(output.lightViewPosition, lightProjectionMatrix);

    // Store the texture coordinates for the pixel shader.
    output.tex = input.tex;
    
    // Calculate the normal vector against the world matrix only.
    output.normal = mul(input.normal, (float3x3) worldMatrix);
	
    // Normalize the normal vector.
    output.normal = normalize(output.normal);

    // Calculate the position of the vertex in the world.
    worldPosition = mul(input.position, worldMatrix);

    // Determine the light position based on the position of the light and the position of the vertex in the world.
    output.lightPos = lightPosition.xyz - worldPosition.xyz;

    // Normalize the light position vector.
    output.lightPos = normalize(output.lightPos);

    return output;
}

float4 ShadowPixelShader(PixelInputType input) : SV_TARGET
{
    float4 color;
    float intensity;

    if (m_isDirection) {
        intensity = saturate(dot(input.normal, -m_lightDirection));
    } else {
        intensity = saturate(dot(input.normal, input.lightPos));
    }
    
    float4 defaultColor = m_diffuseColor * intensity * m_lightIntensity;
    float4 textureColor = shaderTexture.Sample(SampleTypeWrap, input.tex); 
    
    g_ShadowSize = m_ShadowSize;
    g_isSoftShadow = m_isSoftShadow;
    
    color = m_ambientColor + calcShadow(input.lightViewPosition, defaultColor, intensity);
    color = saturate(color) * textureColor;

    return color;
}