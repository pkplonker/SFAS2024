#pragma once
#include "../Math/Vector3.h"


class ILight : public std::enable_shared_from_this<ILight>
{
public:
    struct Light
    {
        Light()
            : Position(0.0f, 0.0f, 0.0f, 1.0f)
              , Direction(0.0f, 0.0f, 1.0f, 0.0f)
              , Color(1.0f, 1.0f, 1.0f, 1.0f)
              , SpotAngle(DirectX::XM_PIDIV2)
              , ConstantAttenuation(1.0f)
              , LinearAttenuation(0.8f)
              , QuadraticAttenuation(0.0f)
              , LightType(DirectionalLight)
              , Enabled(0)
        {
        }

        DirectX::XMFLOAT4 Position;
        DirectX::XMFLOAT4 Direction;
        DirectX::XMFLOAT4 Color;
        float SpotAngle;
        float ConstantAttenuation;
        float LinearAttenuation;
        float QuadraticAttenuation;
        int LightType;
        int Enabled;
        int Padding[2];
    };

    enum LightType
    {
        DirectionalLight = 0,
        PointLight = 1,
        SpotLight = 2
    };

    virtual ~ILight() = default;

    ILight()
    {
    }
    
    virtual Light& GetLight() =0;

protected:
    Light light;
};
