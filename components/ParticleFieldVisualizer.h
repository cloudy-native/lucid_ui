#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <vector>
#include <random>

namespace ui
{

class ParticleFieldVisualizer : public juce::Component, public juce::Timer
{
public:
    ParticleFieldVisualizer();
    ~ParticleFieldVisualizer() override;
    
    void paint(juce::Graphics& g) override;
    void resized() override;
    void timerCallback() override;
    
    // Emitter state updates
    struct EmitterState
    {
        int index;
        juce::String name;
        float countdownProgress;  // 0.0 (just triggered) → 1.0 (about to trigger)
        bool isPlaying;
        float playbackProgress;   // 0.0 (started) → 1.0 (finished)
        float density;            // Events per minute
        juce::Colour color;
        bool enabled;
    };
    
    void setEmitterCount(int count);
    void updateEmitterState(int index, const EmitterState& state);
    void triggerEmitter(int index);  // Call when emitter fires
    
private:
    struct Particle
    {
        juce::Point<float> position;
        juce::Point<float> velocity;
        float life;              // 0.0 (dead) → 1.0 (born)
        float size;
        int targetEmitter;       // Which emitter this particle is attracted to
        juce::Colour color;
        bool isBurst;            // Burst particle (moving away) vs attracted particle
    };
    
    struct EmitterVisual
    {
        juce::Point<float> position;
        float radius;
        juce::Colour color;
        float countdownProgress;
        bool isPlaying;
        float playbackProgress;
        float glowIntensity;     // For pulsing effect
        float burstTime;         // Time since last trigger (for burst animation)
        float density;
        bool enabled;
        juce::String name;
    };
    
    std::vector<EmitterVisual> emitters;
    std::vector<Particle> particles;
    
    std::mt19937 rng;
    std::uniform_real_distribution<float> randomFloat;
    
    // Particle system
    void updateParticles(float deltaTime);
    void spawnParticlesForEmitter(int emitterIndex, int count);
    void createBurstParticles(int emitterIndex);
    
    // Layout
    void layoutEmitters();
    juce::Point<float> getEmitterPosition(int index, int totalCount) const;
    
    // Rendering
    void drawEmitter(juce::Graphics& g, const EmitterVisual& emitter);
    void drawParticle(juce::Graphics& g, const Particle& particle);
    void drawConnections(juce::Graphics& g);
    
    // Animation
    float animationTime = 0.0f;
    juce::int64 lastUpdateTime = 0;
    
    // Constants
    static constexpr int maxParticles = 2000;
    static constexpr float particleLifetime = 3.0f;
    static constexpr float attractionStrength = 100.0f;
    static constexpr float burstDuration = 0.5f;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ParticleFieldVisualizer)
};

} // namespace ui
