#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include "LinearAmplifierPlugin.hpp"
#include <memory>
#include <array>

using namespace DISTRHO;

// Test wrapper to access protected methods
class TestableLinearAmplifierPlugin : public LinearAmplifierPlugin
{
public:
    // Expose protected methods for testing
    using LinearAmplifierPlugin::getParameterValue;
    using LinearAmplifierPlugin::setParameterValue;
    using LinearAmplifierPlugin::run;
};

class PluginTest
{
public:
    PluginTest() : plugin(std::make_unique<TestableLinearAmplifierPlugin>()) {}

protected:
    std::unique_ptr<TestableLinearAmplifierPlugin> plugin;
    static constexpr uint32_t kBufferSize = 64;
    std::array<std::array<float, kBufferSize>, 2> inputBuffers{};
    std::array<std::array<float, kBufferSize>, 2> outputBuffers{};
    std::array<const float*, 2> inputs = {inputBuffers[0].data(), inputBuffers[1].data()};
    std::array<float*, 2> outputs = {outputBuffers[0].data(), outputBuffers[1].data()};
};

TEST_CASE_METHOD(PluginTest, "Plugin creation", "[plugin]")
{
    REQUIRE(plugin != nullptr);
}

TEST_CASE_METHOD(PluginTest, "Parameters", "[parameters]")
{
    REQUIRE(plugin->getParameterValue(kParameterGain) == Catch::Approx(0.0f));

    plugin->setParameterValue(kParameterGain, 6.0f);
    REQUIRE(plugin->getParameterValue(kParameterGain) == Catch::Approx(6.0f));
}

TEST_CASE_METHOD(PluginTest, "Audio processing", "[audio]")
{
    plugin->setParameterValue(kParameterGain, 0.0f);

    for (uint32_t i = 0; i < kBufferSize; ++i) {
        inputBuffers[0][i] = 0.5f;
        inputBuffers[1][i] = 0.5f;
    }

    plugin->run(inputs.data(), outputs.data(), kBufferSize);

    REQUIRE(outputBuffers[0][0] == Catch::Approx(0.5f).epsilon(0.001f));
    REQUIRE(outputBuffers[1][0] == Catch::Approx(0.5f).epsilon(0.001f));
}
