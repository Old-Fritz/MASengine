////////////////////////////////////////////////////////////////////////////////
// Filename: light.ps
////////////////////////////////////////////////////////////////////////////////


/////////////
// GLOBALS //
/////////////
Texture2D shaderTexture;
SamplerState SampleType;

cbuffer ParamsBuffer
{
    float4 selCol;
    float vertPercent;
    float horPercent;
    float selIntensivity;
    float transparency;
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
float4 InterfacePixelShader(PixelInputType input) : SV_TARGET
{
    float4 textureColor;


    // Sample the pixel color from the texture using the sampler at this texture coordinate location.
    textureColor = shaderTexture.Sample(SampleType, input.tex);

     // If the color is black on the texture then treat this pixel as transparent.
    if(textureColor.r == 0.0f)
        textureColor.a = 0.0f;

    //delete some parts of image on horizontal
    if (horPercent < 0) //if < 0, delete left part]
    {
        if (input.tex.x <= horPercent + 1.0f)
            textureColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
    }
    else
    {
        if (input.tex.x >= horPercent)
            textureColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
    }
        

    //delete some parts of image on vertical
    if (vertPercent < 0) //if < 0, delete left part
    {
        if (input.tex.y <= vertPercent + 1.0f)
            textureColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
    }
    else
    {
        if (input.tex.y >= vertPercent)
            textureColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
    }

    //save alpha
    float lastA = textureColor.a;

    //calculate sel intensivity
    if ((textureColor.r != 0.0f) || (textureColor.g != 0.0f) || (textureColor.b != 0.0f))
    {
		//textureColor1 = lerp(textureColor1, selCol, (abs(input.tex.x - 0.5f)*2.0f)*(abs(input.tex.y - 0.5f)*2.0f)*selIntensivity);
        textureColor = lerp(textureColor, selCol, sqrt((input.tex.x - 0.5f) * (input.tex.x - 0.5f) + (input.tex.y - 0.5f) * (input.tex.y - 0.5f)) * selIntensivity * 0.9f);
    }

    //calculate alpha
    textureColor.a = lastA * transparency;

    return textureColor;
}