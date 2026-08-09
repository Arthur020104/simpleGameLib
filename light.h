#pragma once
#include <component.h>
#include <program.h>


class Light: public Component
{
  public:
    Light(cy::Vec3f position, cy::Vec3f color, float intensity ): Component(position), color(color), intensity(intensity) {};

    ~Light() = default;

    virtual void start() override {};
    virtual void beforeUpdate() override {};
    virtual void aftherUpdate() override {};

    virtual void bindUniforms(Program* shaderProgram);
    virtual void bindUniforms(Program* shaderProgram, char* arrayName, uint16_t index);
    virtual uint16_t getLightType() { return 0;};

    cy::Vec3f color;
    float intensity;
    float ambientIntensity = 0.14f;
  private:
};

class DirectionalLight: public Light
{
  public:
    using Light::Light;
    
    virtual cy::Vec3f getPosition() override;

    virtual uint16_t getLightType() { return DirectionalLight::TYPE;};
  private:
    static const uint16_t TYPE = 1;
};

class PointLight: public Light
{
  public:
    using Light::Light;

    virtual uint16_t getLightType() { return PointLight::TYPE;};

    virtual void bindUniforms(Program* shaderProgram) override;
    virtual void bindUniforms(Program* shaderProgram, char* arrayName, uint16_t index) override;

    float constantFallOff = 1.0f;
    float linearFallOff = 0.09f;
    float quadraticFallOff = 0.032f;
  private:
    static const uint16_t TYPE = 2;
};
