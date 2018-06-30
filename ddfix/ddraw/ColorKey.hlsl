float4 srcColorKey;
float4 destColorKey;
bool haveColorKey[2];
bool checkAlpha;


sampler2D sampler0;
 
struct PS_INPUT 
{
   float2 Texcoord : TEXCOORD0;
   float4 Texcolor : COLOR;
};
 
struct PS_OUTPUT 
{
   float4 Texcolor : COLOR;
};

 
PS_OUTPUT ps_main( PS_INPUT Input )
{
   PS_OUTPUT OUT;
   float4 srcPixel = tex2D(sampler0, Input.Texcoord );
   float4 destPixel = srcPixel;

   if (checkAlpha)
   {
	   if (srcPixel.a == 0.0)
	   {
		   destPixel.a = 0.0;
	   }
	   else
	   {
		   destPixel.a = 1.0;
	   }
   }
   else if (haveColorKey[0])
   {
		if (abs(srcPixel.r - srcColorKey.r) < 0.01 &&
			abs(srcPixel.g - srcColorKey.g) < 0.01 &&
			abs(srcPixel.b - srcColorKey.b) < 0.01)
		{
			destPixel.a = 0.0;
		}
		else
		{
			destPixel.a = 1.0;
		}
   }
   else
   {
	   destPixel.a = 1.0;
   }
   
   OUT.Texcolor = destPixel;
   
   return OUT;
}