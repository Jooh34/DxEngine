float4 main() : SV_TARGET
{
	return float4(1.0f, 1.0f, 1.0f, 1.0f);
}
/////////////
// GLOBALS //
/////////////
Texture2D shaderTexture;
SamplerState SampleType;

cbuffer LightBuffer
{
    float4 ambientColor;
    float4 diffuseColor;
    float3 lightDirection;
    float specularPower;
    float4 specularColor;
};


//////////////
// TYPEDEFS //
//////////////
struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float3 viewDirection : TEXCOORD1;
};


////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 main(PixelInputType input) : SV_TARGET
{
    float4 textureColor;
    float3 lightDir;
    float lightIntensity;
    float4 color;
    float3 reflection;
    float4 specular;


    // Sample the pixel color from the texture using the sampler at this texture coordinate location.
    textureColor = shaderTexture.Sample(SampleType, input.tex);
	//This is where the lighting equation that was discussed earlier is now implemented.The light intensity value is calculated as the dot product between the normal vector of triangle and the light direction vector.

    // Set the default output color to the ambient light value for all pixels.
    color = ambientColor;

    // Initialize the specular color.
    specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// Invert the light direction for calculations.
    lightDir = -lightDirection;

    // Blinn-Phong Reflection
    float3 halfwayDir = normalize(input.viewDirection + lightDir);
    
    lightIntensity = saturate(dot(input.normal, lightDir));
    
    if (lightIntensity > 0.0f)
    {
        // Determine the final diffuse color based on the diffuse color and the amount of light intensity.
        color += (diffuseColor * lightIntensity);
        color = saturate(color);
    }
      
    specular = pow(saturate(dot(input.normal, halfwayDir)), specularPower);

	// Multiply the texture pixel and the final diffuse color to get the final pixel color result.
    color = color * textureColor;

    // Add the specular component last to the output color.
    color = saturate(color + specular);

    return color;
}
