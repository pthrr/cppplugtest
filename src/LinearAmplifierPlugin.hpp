#pragma once

#include "DistrhoPlugin.hpp"
#include <algorithm>
#include <cmath>
#include <span>

START_NAMESPACE_DISTRHO

class LinearAmplifierPlugin : public Plugin
{
public:
    LinearAmplifierPlugin();

protected:
    const char* getLabel() const override;
    const char* getDescription() const override;
    const char* getMaker() const override;
    const char* getHomePage() const override;
    const char* getLicense() const override;
    uint32_t getVersion() const override;
    int64_t getUniqueId() const override;

    void initParameter( uint32_t index, Parameter& parameter ) override;
    float getParameterValue( uint32_t index ) const override;
    void setParameterValue( uint32_t index, float value ) override;
    void run( const float** inputs, float** outputs, uint32_t frames ) override;
    void sampleRateChanged( double newSampleRate ) override;

private:
    float m_gain;
    float m_gainDb;

    DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR( LinearAmplifierPlugin )
};

END_NAMESPACE_DISTRHO
