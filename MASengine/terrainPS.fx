////////////////////////////////////////////////////////////////////////////////
// Filename: light.ps
////////////////////////////////////////////////////////////////////////////////


/////////////
// GLOBALS //
/////////////
Texture2D shaderTexture;
SamplerState SampleType[2];

cbuffer ParamsBuffer
{
    float3 lightDirection;
    float4 ambientColor;
    float4 diffuseColor;
    float3 cameraPosition;
    float4 specularColor;
    float specularPower;
	float provsColor[256];
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
float4 TerrainPixelShader(PixelInputType input) : SV_TARGET
{
    float2 squereCoord;
    float3 lightDir;
    float lightIntensity;
    float4 color;
    float4 provColor;
    float4 provColor2;
    float4 tempColor;
    float3 reflection;
    float4 specular;
    float4 yellow;
    int provnum;
    int provnum2;
    float2 mapCoord;

	///CALCULATE BORDERS///
    provColor = shaderTexture.Sample(SampleType[0], input.tex);


    provColor2 = shaderTexture.Sample(SampleType[1], input.tex);


    provnum2 = provColor2.z * 255.01f;

    provnum = provColor.z * 255.01f;

	
    if (provColor.x > 0 || provColor.y > 0)
    {
        if (provColor.x >= 0.9f || provColor.y >= 0.9f)
        {
            color = float4(0.3f, 0.3f, 0.7f, 1.0f);
            provnum = 400;
        }
        else if (provColor.x > 0.25f || provColor.y > 0.25f)
        {
            provColor = float4(0.4f, 0.4f, 0.4f, 1);
            provnum = 300;
        }
        else
        {
            provColor = provColor2;
            provnum = provnum2;
            if (provColor.x > 0 || provColor.y > 0)
            {
                provColor = float4(0.4f, 0.4f, 0.4f, 1);
                provnum = 300;
            }
        }
    }
    else
    {
        provColor = provColor2;
        provnum = provnum2;
    }
	

	///CALCULATE LIGHT///



	// Set the default output color to the ambient light value for all pixels.
    color = ambientColor;

	// Initialize the specular color.
    specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// Invert the light direction for calculations.
    lightDir = -lightDirection;

	// Calculate the amount of light on this pixel.
    lightIntensity = saturate(dot(input.normal, lightDir));

    if (lightIntensity > 0.0f)
    {
		// Determine the final diffuse color based on the diffuse color and the amount of light intensity.
        color += (diffuseColor * lightIntensity);
    }

	// Saturate the ambient and diffuse color.
    color = saturate(color);

	// Calculate the reflection vector based on the light intensity, normal vector, and light direction.
    reflection = normalize(2 * lightIntensity * input.normal - lightDir);

    yellow = float4(1.0f, 1.0f, 0.0f, 0.0f);


	///CALCULATE FINAL///

    if (provnum < 255)
    {
        provColor = provsColor[provnum];
        provColor = lerp(provColor, yellow, provColor.w / 1.6f);
    }
    color = lerp(color, provColor, 0.55f);
    color.w = 1.0f;
	
    if (provnum == 400)
        color = float4(0.3f, 0.3f, 0.7f, 1.0f);

    return color;
}