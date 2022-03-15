float4 main(float2 inPos : POSITION) : SV_POSITION {
    // x y z w
    return float4(inPos, 0, 1);
}