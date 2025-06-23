#include "LinearAmplifierPlugin.hpp"

#ifdef PLUGIN_HAS_LOGGING
#include <spdlog/spdlog.h>
#endif

START_NAMESPACE_DISTRHO

LinearAmplifierPlugin::LinearAmplifierPlugin()
    : Plugin( kParameterCount, 0, 0 )
    , m_gain( 1.0f )
    , m_gainDb( 0.0f )
{
    setParameterValue( kParameterGain, 0.0f );
#ifdef PLUGIN_HAS_LOGGING
    spdlog::info( "LinearAmplifierPlugin initialized" );
#endif
}

const char* LinearAmplifierPlugin::getLabel() const
{
    return "LinearAmp";
}
const char* LinearAmplifierPlugin::getDescription() const
{
    return "Simple Linear Amplifier";
}
const char* LinearAmplifierPlugin::getMaker() const
{
    return "DPF Example";
}
const char* LinearAmplifierPlugin::getHomePage() const
{
    return "https://github.com/example/linear-amplifier";
}
const char* LinearAmplifierPlugin::getLicense() const
{
    return "MIT";
}
uint32_t LinearAmplifierPlugin::getVersion() const
{
    return d_version( 1, 0, 0 );
}
int64_t LinearAmplifierPlugin::getUniqueId() const
{
    return d_cconst( 'L', 'A', 'm', 'p' );
}

void LinearAmplifierPlugin::initParameter( uint32_t index, Parameter& parameter )
{
    if( index == kParameterGain ) {
        parameter.hints = kParameterIsAutomatable | kParameterIsLogarithmic;
        parameter.name = "Gain";
        parameter.symbol = "gain";
        parameter.unit = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -60.0f;
        parameter.ranges.max = 20.0f;
    }
}

float LinearAmplifierPlugin::getParameterValue( uint32_t index ) const
{
    return ( index == kParameterGain ) ? m_gainDb : 0.0f;
}

void LinearAmplifierPlugin::setParameterValue( uint32_t index, float value )
{
    if( index == kParameterGain ) {
        m_gainDb = value;
        m_gain = std::pow( 10.0f, value / 20.0f );
#ifdef PLUGIN_HAS_LOGGING
        spdlog::debug( "Gain set to {:.2f} dB", m_gainDb );
#endif
    }
}

void LinearAmplifierPlugin::run( const float** inputs, float** outputs, uint32_t frames )
{
    const auto processChannel = [this]( const float* input, float* output, uint32_t count ) noexcept {
        auto input_range = std::span{ input, count };
        auto output_range = std::span{ output, count };
        std::transform( input_range.begin(), input_range.end(), output_range.begin(),
            [gain = this->m_gain]( float sample ) noexcept -> float { return sample * gain; } );
    };

    processChannel( inputs[0], outputs[0], frames );
    processChannel( inputs[1], outputs[1], frames );
}

void LinearAmplifierPlugin::sampleRateChanged( double newSampleRate )
{
    // Handle sample rate changes if needed
    (void)newSampleRate;
}

Plugin* createPlugin()
{
    return new LinearAmplifierPlugin();
}

END_NAMESPACE_DISTRHO
