////////////////////////////////////////////////////////////////////////////////
// Filename: fontPS.fx
////////////////////////////////////////////////////////////////////////////////


/////////////
// GLOBALS //
/////////////
Texture2D shaderTexture;
SamplerState SampleType;

cbuffer ParamsBuffer
{
    float4 textColor;
};


//////////////
// TYPEDEFS //
//////////////

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};


////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 pixelShader(PixelInputType input) : SV_TARGET
{
    float4 textureColor;


    // Sample the pixel color from the texture using the sampler at this texture coordinate location.
    textureColor = shaderTexture.Sample(SampleType, input.tex);

     // If the color is black on the texture then treat this pixel as transparent.
    if (textureColor.r <= 0.2f)
    {
        textureColor.a = 0.0f;
    }
    else
    {
        textureColor = textColor;
    }

    return textureColor;
}