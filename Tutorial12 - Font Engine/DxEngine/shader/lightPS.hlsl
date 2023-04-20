
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
float4 LightPixelShader(PixelInputType input) : SV_TARGET
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

	// Calculate the amount of light on this pixel.
	lightIntensity = saturate(dot(input.normal, lightDir));
	//And finally the diffuse value of the light is combined with the texture pixel value to produce the color result.
    
    if (lightIntensity > 0.0f)
    {
        // Determine the final diffuse color based on the diffuse color and the amount of light intensity.
        color += (diffuseColor * lightIntensity);

        // Saturate the ambient and diffuse color.
        color = saturate(color);
        //The reflection vector for specular lighting is calculated here in the pixel shader provided the light intensity is greater than zero.This is the same equation as listed at the beginning of the tutorial.

		// Calculate the reflection vector based on the light intensity, normal vector, and light direction.
		reflection = normalize(2 * lightIntensity * input.normal - lightDir);
        //The amount of specular light is then calculated using the reflection vector and the viewing direction.The smaller the angle between the viewer and the light source the greater the specular light reflection will be.The result is taken to the power of the specularPower value.The lower the specularPower value the greater the final effect is.

		// Determine the amount of specular light based on the reflection vector, viewing direction, and specular power.
		specular = pow(saturate(dot(reflection, input.viewDirection)), specularPower);
    }

	// Determine the final amount of diffuse color based on the diffuse color combined with the light intensity.
	color = saturate(color);

	// Multiply the texture pixel and the final diffuse color to get the final pixel color result.
	color = color * textureColor;

    // Add the specular component last to the output color.
    color = saturate(color + specular);

	return color;
}
