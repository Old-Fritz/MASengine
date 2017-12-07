////////////////////////////////////////////////////////////////////////////////
// Filename: light.ps
////////////////////////////////////////////////////////////////////////////////


/////////////
// GLOBALS //
/////////////
Texture2D textures[12];
SamplerState SampleType[3];

//float g_sizeOfShaderTexture = 128;

cbuffer ParamsBuffer
{
    float4 ambientColor;
    float4 diffuseColor;
    float4 specularColor;
	float4 provsColor[256];
	float3 lightDirection;
	float specularPower;
	float3 cameraPosition;

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
	float clip : SV_ClipDistance0;
};

float4 CalculateLight(float3 normal, float3 viewDirection);
float4 CalculatePhysicalMap(PixelInputType input);
float4 CalculateCurrentPhysical(float4 color, float2 mapCoord);
bool NearColor(float4 color1, float4 color2);
float4 ConvertRGB256(float r, float g, float b);

////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 pixelShader(PixelInputType input) : SV_TARGET
{
	float4 terrainColor;
	float4 lightColor;
    float4 color;
    float4 provColor;
    float4 provColor2;
    float4 tempColor;
    int provnum;
    int provnum2;
    float2 mapCoord;

	///CALCULATE BORDERS///
    provColor = textures[0].Sample(SampleType[0], input.tex);
	provColor2 = textures[0].Sample(SampleType[1], input.tex);

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
	lightColor = CalculateLight(input.normal,input.viewDirection);
	//return color;


	terrainColor = CalculatePhysicalMap(input);

	///CALCULATE FINAL///

    if (provnum < 255)
    {
        provColor = provsColor[provnum];
        //provColor = lerp(provColor, yellow, provColor.w / 1.6f);
    }
	color = lerp(terrainColor, provColor, 0.6f);
    //color = lerp(lightColor, color, 0.35f);
	color = color * lightColor;
    color.w = 1.0f;
	
   if (provnum == 400)
        color = terrainColor * lightColor;

    //return textures[1].Sample(SampleType[1], input.tex);
	return  color;
}


float4 CalculateLight(float3 normal,float3 viewDirection)
{
	float4 color;
	float4 specular;
	float3 lightDir;
	float lightIntensity;
	float3 reflection;

	// Set the default output color to the ambient light value for all pixels.
	color = ambientColor;

	// Initialize the specular color.
	specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// Invert the light direction for calculations.
	lightDir = -lightDirection;

	// Calculate the amount of light on this pixel.
	lightIntensity = saturate(dot(normal, lightDir));

	if (lightIntensity > 0.0f)
	{
		// Determine the final diffuse color based on the diffuse color and the amount of light intensity.
		color += (diffuseColor * lightIntensity);

		// Saturate the ambient and diffuse color.
		color = saturate(color);

		// Calculate the reflection vector based on the light intensity, normal vector, and light direction.
		reflection = normalize(2 * lightIntensity * normal - lightDir);

		// Determine the amount of specular light based on the reflection vector, viewing direction, and specular power.
		specular = pow(saturate(dot(reflection, viewDirection)), specularPower);
	}

	// Add the specular component last to the output color.
	color = saturate(color + specular);

	return color;

	
}

float4 CalculatePhysicalMap(PixelInputType input)
{
	float4 textureColor, textureColorLeft, textureColorRight, textureColorUp, textureColorDown;
	float2 texCoordLeft, texCoordRight, texCoordUp, texCoordDown;
	float2 squereCoord, mapCoord;
	float sizeOfShaderTexture = 128;

	// Sample the pixel color from the texture using the sampler at this texture coordinate location.
	//textureColor = textures[1].Sample(SampleType[1], input.tex*(sizeOfShaderTexture / 129.0f));
	textureColor = textures[1].Sample(SampleType[1], (input.tex*sizeOfShaderTexture + 1.0f) / (sizeOfShaderTexture + 2.0f) - 0.00003f);
	squereCoord.x = (input.tex.x*sizeOfShaderTexture) - (int)(input.tex.x*sizeOfShaderTexture);
	squereCoord.y = (input.tex.y*sizeOfShaderTexture) - (int)(input.tex.y*sizeOfShaderTexture);
	//squereCoord = (input.tex*sizeOfShaderTexture + 1.0f);
	texCoordLeft = texCoordRight = texCoordUp = texCoordDown = (input.tex*sizeOfShaderTexture + 1.0f) / (sizeOfShaderTexture + 2.0f) - 0.00003f;
	texCoordLeft.x -= 1.0f / (sizeOfShaderTexture + 2.0f);
	texCoordRight.x += 1.0f / (sizeOfShaderTexture + 2.0f);
	texCoordUp.y -= 1.0f / (sizeOfShaderTexture + 2.0f);
	texCoordDown.y += 1.0f / (sizeOfShaderTexture + 2.0f);
	textureColorLeft = textures[1].Sample(SampleType[1], texCoordLeft);
	textureColorRight = textures[1].Sample(SampleType[1], texCoordRight);
	textureColorUp = textures[1].Sample(SampleType[1], texCoordUp);
	textureColorDown = textures[1].Sample(SampleType[1], texCoordDown);
	//return textureColorLeft;

	mapCoord = input.tex * 16;

	textureColor = CalculateCurrentPhysical(textureColor, mapCoord);
	//return textureColor;
	textureColorLeft = CalculateCurrentPhysical(textureColorLeft, mapCoord);
	textureColorRight = CalculateCurrentPhysical(textureColorRight, mapCoord);
	textureColorUp = CalculateCurrentPhysical(textureColorUp, mapCoord);
	textureColorDown = CalculateCurrentPhysical(textureColorDown, mapCoord);

	float4 horColor;
	if (squereCoord.x > 0.5f)
		horColor = textureColor*(1.5f - squereCoord.x) + textureColorRight*((squereCoord.x - 0.5f));
	else
		//horColor = textureColor;
		horColor = textureColor*((squereCoord.x + 0.5f)) + textureColorLeft*(0.5f - squereCoord.x);

	float4 verColor;
	if (squereCoord.y > 0.5f)
		horColor = horColor*(1.5f - squereCoord.y) + textureColorDown*((squereCoord.y - 0.5f));
	else
		//horColor = textureColor;
		horColor = horColor*((squereCoord.y + 0.5f)) + textureColorUp*(0.5f - squereCoord.y);

	textureColor = horColor;

	return textureColor;


}

float4 CalculateCurrentPhysical(float4 color, float2 mapCoord)
{
	float4 textureColor;

		
	if (NearColor(color, ConvertRGB256(0.0f, 165.0f, 232.0f))) //sea
		textureColor = textures[2].Sample(SampleType[2], mapCoord);
	else if (NearColor(color, ConvertRGB256(243.0f, 237.0f, 99.0f))) //beach
		textureColor = textures[3].Sample(SampleType[2], mapCoord);
	else if (NearColor(color, ConvertRGB256(0, 135.0f, 71.0f))) //forest
		textureColor = textures[4].Sample(SampleType[2], mapCoord);
	else if (NearColor(color, ConvertRGB256(161.0f, 194.0f, 49.0f))) //hills
		textureColor = textures[5].Sample(SampleType[2], mapCoord);
	else if (NearColor(color, ConvertRGB256(128.0f, 104.0f, 24.0f))) //middle mountains
		textureColor = textures[6].Sample(SampleType[2], mapCoord);
	else if (NearColor(color, ConvertRGB256(128.0f, 104.0f, 103.0f))) //big mountains
		textureColor = textures[7].Sample(SampleType[2], mapCoord);
	else if (NearColor(color, ConvertRGB256(30, 30, 30))) //cities
		textureColor = textures[8].Sample(SampleType[2], mapCoord);
	else
		textureColor = textures[2].Sample(SampleType[2], mapCoord);

	return textureColor;
}

bool NearColor(float4 color1, float4 color2)
{
	return (abs(color1.x - color2.x) < 0.08f && abs(color1.y - color2.y) < 0.08f && abs(color1.z - color2.z) < 0.08f);
}

float4 ConvertRGB256(float r, float g, float b)
{
	return float4(r / 256.0f, g / 256.0f, b / 256.0f, 1.0f);
}