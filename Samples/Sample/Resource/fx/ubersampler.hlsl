
texture diffuseTexture;
sampler diffuseSampler = sampler_state
{
    Texture = <diffuseTexture>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;

    AddressU = Wrap;
    AddressV = Wrap;
};
