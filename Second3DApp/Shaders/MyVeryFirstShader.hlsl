struct VS_IN
{
	float4 pos : POSITION0;
	float4 col : COLOR0;
};

struct PS_IN
{
	float4 pos : SV_POSITION;
 	float4 col : COLOR;
};

cbuffer ConstBuf : register(b0)
{
    matrix transformations;
    matrix view;
    matrix projection;
    float4 color;
}

PS_IN VSMain( VS_IN input )
{
	PS_IN output = (PS_IN)0;
	
    float4 pos = float4(input.pos.xyz, 1.0f);
    
    pos = mul(pos, transformations);
    pos = mul(pos, view);
    pos = mul(pos, projection);
    
    output.pos = pos;
    
    
    output.col = (input.col + color);
	
	return output;
}

float4 PSMain( PS_IN input ) : SV_Target
{
	float4 col = input.col;
//#ifdef TEST
//	if (input.pos.x > 400) col = TCOLOR;
//#endif
	return col;
}