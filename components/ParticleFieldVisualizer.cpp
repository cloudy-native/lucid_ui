#include "ParticleFieldVisualizer.h"
#include <cmath>

namespace ui
{

ParticleFieldVisualizer::ParticleFieldVisualizer()
    : rng(std::random_device{}()),
      randomFloat(0.0f, 1.0f)
{
    startTimerHz(60);  // 60 FPS updates
    lastUpdateTime = juce::Time::getMillisecondCounterHiRes();
}

ParticleFieldVisualizer::~ParticleFieldVisualizer()
{
    stopTimer();
}

void ParticleFieldVisualizer::paint(juce::Graphics& g)
{
    // Dark background with subtle gradient
    g.fillAll(juce::Colour(0xff0a0a0a));
    
    auto bounds = getLocalBounds().toFloat();
    juce::ColourGradient gradient(
        juce::Colour(0xff0f0f0f), bounds.getCentreX(), bounds.getY(),
        juce::Colour(0xff050505), bounds.getCentreX(), bounds.getBottom(),
        false
    );
    g.setGradientFill(gradient);
    g.fillRect(bounds);
    
    // Enable antialiasing for smooth rendering
    g.setOpacity(1.0f);
    
    // Draw connections between emitters (subtle web)
    drawConnections(g);
    
    // Draw particles (behind emitters)
    for (const auto& particle : particles)
    {
        if (particle.life > 0.0f)
            drawParticle(g, particle);
    }
    
    // Draw emitters (on top)
    for (const auto& emitter : emitters)
    {
        if (emitter.enabled)
            drawEmitter(g, emitter);
    }
}

void ParticleFieldVisualizer::resized()
{
    layoutEmitters();
}

void ParticleFieldVisualizer::timerCallback()
{
    auto currentTime = juce::Time::getMillisecondCounterHiRes();
    float deltaTime = static_cast<float>(currentTime - lastUpdateTime) / 1000.0f;
    lastUpdateTime = currentTime;
    
    // Clamp delta time to avoid huge jumps
    deltaTime = juce::jmin(deltaTime, 0.1f);
    
    animationTime += deltaTime;
    
    // Update particle physics
    updateParticles(deltaTime);
    
    // Spawn new particles based on emitter states
    for (size_t i = 0; i < emitters.size(); ++i)
    {
        auto& emitter = emitters[i];
        
        if (!emitter.enabled)
            continue;
        
        // Decay burst animation
        if (emitter.burstTime > 0.0f)
        {
            emitter.burstTime -= deltaTime;
            if (emitter.burstTime < 0.0f)
                emitter.burstTime = 0.0f;
        }
        
        // Pulsing glow effect
        emitter.glowIntensity = 0.3f + 0.2f * std::sin(animationTime * 2.0f + i * 0.5f);
        
        // Spawn particles attracted to this emitter
        // More particles = closer to trigger time
        float spawnRate = emitter.density * (1.0f - emitter.countdownProgress) * deltaTime;
        int particlesToSpawn = static_cast<int>(spawnRate * 10.0f);
        
        if (particlesToSpawn > 0 && particles.size() < maxParticles)
        {
            spawnParticlesForEmitter(static_cast<int>(i), particlesToSpawn);
        }
    }
    
    repaint();
}

void ParticleFieldVisualizer::setEmitterCount(int count)
{
    emitters.resize(count);
    
    // Initialize emitters with default values
    for (int i = 0; i < count; ++i)
    {
        emitters[i].radius = 20.0f;
        emitters[i].color = juce::Colour(0xff00aaff);
        emitters[i].countdownProgress = 0.5f;
        emitters[i].isPlaying = false;
        emitters[i].playbackProgress = 0.0f;
        emitters[i].glowIntensity = 0.5f;
        emitters[i].burstTime = 0.0f;
        emitters[i].density = 2.0f;
        emitters[i].enabled = true;
        emitters[i].name = "Emitter " + juce::String(i + 1);
    }
    
    layoutEmitters();
}

void ParticleFieldVisualizer::updateEmitterState(int index, const EmitterState& state)
{
    if (index < 0 || index >= static_cast<int>(emitters.size()))
        return;
    
    auto& emitter = emitters[index];
    emitter.countdownProgress = state.countdownProgress;
    emitter.isPlaying = state.isPlaying;
    emitter.playbackProgress = state.playbackProgress;
    emitter.density = state.density;
    emitter.color = state.color;
    emitter.enabled = state.enabled;
    emitter.name = state.name;
}

void ParticleFieldVisualizer::triggerEmitter(int index)
{
    if (index < 0 || index >= static_cast<int>(emitters.size()))
        return;
    
    auto& emitter = emitters[index];
    emitter.burstTime = burstDuration;
    
    // Create burst particles
    createBurstParticles(index);
}

void ParticleFieldVisualizer::updateParticles(float deltaTime)
{
    auto bounds = getLocalBounds().toFloat();
    
    for (auto& particle : particles)
    {
        if (particle.life <= 0.0f)
            continue;
        
        // Age particle
        particle.life -= deltaTime / particleLifetime;
        
        if (particle.life <= 0.0f)
            continue;
        
        if (particle.isBurst)
        {
            // Burst particles move away and fade
            particle.position += particle.velocity * deltaTime;
            particle.velocity *= 0.95f;  // Damping
        }
        else
        {
            // Attracted particles move toward target emitter
            if (particle.targetEmitter >= 0 && particle.targetEmitter < static_cast<int>(emitters.size()))
            {
                auto& target = emitters[particle.targetEmitter];
                
                if (target.enabled)
                {
                    auto direction = target.position - particle.position;
                    float distance = direction.getDistanceFromOrigin();
                    
                    if (distance > 1.0f)
                    {
                        direction /= distance;  // Normalize
                        
                        // Acceleration toward target
                        float strength = attractionStrength * (1.0f - target.countdownProgress);
                        particle.velocity += direction * strength * deltaTime;
                        
                        // Limit velocity
                        float speed = particle.velocity.getDistanceFromOrigin();
                        if (speed > 200.0f)
                        {
                            particle.velocity *= 200.0f / speed;
                        }
                    }
                    else
                    {
                        // Reached target - kill particle
                        particle.life = 0.0f;
                    }
                }
            }
            
            particle.position += particle.velocity * deltaTime;
            particle.velocity *= 0.98f;  // Slight damping
        }
        
        // Kill particles outside bounds
        if (!bounds.contains(particle.position))
        {
            particle.life = 0.0f;
        }
    }
    
    // Remove dead particles (keep vector compact)
    particles.erase(
        std::remove_if(particles.begin(), particles.end(),
            [](const Particle& p) { return p.life <= 0.0f; }),
        particles.end()
    );
}

void ParticleFieldVisualizer::spawnParticlesForEmitter(int emitterIndex, int count)
{
    if (emitterIndex < 0 || emitterIndex >= static_cast<int>(emitters.size()))
        return;
    
    auto& emitter = emitters[emitterIndex];
    auto bounds = getLocalBounds().toFloat();
    
    for (int i = 0; i < count && particles.size() < maxParticles; ++i)
    {
        Particle p;
        
        // Spawn from random edge of screen
        float edge = randomFloat(rng) * 4.0f;
        if (edge < 1.0f)
        {
            // Top edge
            p.position = juce::Point<float>(randomFloat(rng) * bounds.getWidth(), 0.0f);
        }
        else if (edge < 2.0f)
        {
            // Right edge
            p.position = juce::Point<float>(bounds.getWidth(), randomFloat(rng) * bounds.getHeight());
        }
        else if (edge < 3.0f)
        {
            // Bottom edge
            p.position = juce::Point<float>(randomFloat(rng) * bounds.getWidth(), bounds.getHeight());
        }
        else
        {
            // Left edge
            p.position = juce::Point<float>(0.0f, randomFloat(rng) * bounds.getHeight());
        }
        
        p.velocity = juce::Point<float>(0.0f, 0.0f);
        p.life = 1.0f;
        p.size = 1.0f + randomFloat(rng) * 2.0f;
        p.targetEmitter = emitterIndex;
        p.color = emitter.color.withAlpha(0.6f);
        p.isBurst = false;
        
        particles.push_back(p);
    }
}

void ParticleFieldVisualizer::createBurstParticles(int emitterIndex)
{
    if (emitterIndex < 0 || emitterIndex >= static_cast<int>(emitters.size()))
        return;
    
    auto& emitter = emitters[emitterIndex];
    
    // Create radial burst
    int burstCount = 30 + static_cast<int>(emitter.density * 10.0f);
    
    for (int i = 0; i < burstCount && particles.size() < maxParticles; ++i)
    {
        Particle p;
        
        float angle = (i / static_cast<float>(burstCount)) * juce::MathConstants<float>::twoPi;
        float speed = 50.0f + randomFloat(rng) * 100.0f;
        
        p.position = emitter.position;
        p.velocity = juce::Point<float>(std::cos(angle), std::sin(angle)) * speed;
        p.life = 1.0f;
        p.size = 2.0f + randomFloat(rng) * 3.0f;
        p.targetEmitter = -1;
        p.color = emitter.color.brighter(0.5f);
        p.isBurst = true;
        
        particles.push_back(p);
    }
}

void ParticleFieldVisualizer::layoutEmitters()
{
    if (emitters.empty())
        return;
    
    auto bounds = getLocalBounds().toFloat().reduced(60.0f);
    int count = static_cast<int>(emitters.size());
    
    for (int i = 0; i < count; ++i)
    {
        emitters[i].position = getEmitterPosition(i, count);
    }
}

juce::Point<float> ParticleFieldVisualizer::getEmitterPosition(int index, int totalCount) const
{
    auto bounds = getLocalBounds().toFloat().reduced(60.0f);
    
    if (totalCount == 1)
    {
        return bounds.getCentre();
    }
    else if (totalCount <= 6)
    {
        // Circular layout
        float angle = (index / static_cast<float>(totalCount)) * juce::MathConstants<float>::twoPi;
        float radius = juce::jmin(bounds.getWidth(), bounds.getHeight()) * 0.35f;
        
        return bounds.getCentre() + juce::Point<float>(
            std::cos(angle) * radius,
            std::sin(angle) * radius
        );
    }
    else
    {
        // Grid layout
        int cols = static_cast<int>(std::ceil(std::sqrt(totalCount)));
        int row = index / cols;
        int col = index % cols;
        
        float cellWidth = bounds.getWidth() / cols;
        float cellHeight = bounds.getHeight() / ((totalCount + cols - 1) / cols);
        
        return juce::Point<float>(
            bounds.getX() + (col + 0.5f) * cellWidth,
            bounds.getY() + (row + 0.5f) * cellHeight
        );
    }
}

void ParticleFieldVisualizer::drawEmitter(juce::Graphics& g, const EmitterVisual& emitter)
{
    auto pos = emitter.position;
    float radius = emitter.radius;
    
    // Outer glow (larger when playing or about to trigger)
    float glowRadius = radius * (1.5f + emitter.glowIntensity);
    
    if (emitter.isPlaying)
    {
        // Pulsing halo when playing
        float playPulse = 1.0f + 0.3f * std::sin(animationTime * 8.0f);
        glowRadius *= playPulse;
    }
    
    if (emitter.burstTime > 0.0f)
    {
        // Expanding ring on trigger
        float burstProgress = 1.0f - (emitter.burstTime / burstDuration);
        float ringRadius = radius * (1.0f + burstProgress * 3.0f);
        float ringAlpha = (1.0f - burstProgress) * 0.8f;
        
        g.setColour(emitter.color.withAlpha(ringAlpha));
        g.drawEllipse(pos.x - ringRadius, pos.y - ringRadius,
                     ringRadius * 2.0f, ringRadius * 2.0f, 2.0f);
    }
    
    // Glow
    juce::ColourGradient glow(
        emitter.color.withAlpha(0.4f * emitter.glowIntensity), pos.x, pos.y,
        emitter.color.withAlpha(0.0f), pos.x + glowRadius, pos.y,
        true
    );
    g.setGradientFill(glow);
    g.fillEllipse(pos.x - glowRadius, pos.y - glowRadius,
                 glowRadius * 2.0f, glowRadius * 2.0f);
    
    // Core
    g.setColour(emitter.color);
    g.fillEllipse(pos.x - radius, pos.y - radius, radius * 2.0f, radius * 2.0f);
    
    // Countdown ring
    if (emitter.countdownProgress > 0.0f)
    {
        float ringRadius = radius * 1.2f;
        float startAngle = -juce::MathConstants<float>::halfPi;
        float endAngle = startAngle + (emitter.countdownProgress * juce::MathConstants<float>::twoPi);
        
        juce::Path arc;
        arc.addCentredArc(pos.x, pos.y, ringRadius, ringRadius,
                         0.0f, startAngle, endAngle, true);
        
        g.setColour(emitter.color.brighter(0.3f).withAlpha(0.7f));
        g.strokePath(arc, juce::PathStrokeType(3.0f));
    }
    
    // Playback progress (inner ring)
    if (emitter.isPlaying && emitter.playbackProgress > 0.0f)
    {
        float innerRadius = radius * 0.6f;
        float startAngle = -juce::MathConstants<float>::halfPi;
        float endAngle = startAngle + (emitter.playbackProgress * juce::MathConstants<float>::twoPi);
        
        juce::Path arc;
        arc.addCentredArc(pos.x, pos.y, innerRadius, innerRadius,
                         0.0f, startAngle, endAngle, true);
        
        g.setColour(juce::Colours::white.withAlpha(0.5f));
        g.strokePath(arc, juce::PathStrokeType(2.0f));
    }
}

void ParticleFieldVisualizer::drawParticle(juce::Graphics& g, const Particle& particle)
{
    float alpha = particle.life;
    
    if (particle.isBurst)
    {
        // Burst particles fade quickly
        alpha *= 0.6f;
    }
    
    g.setColour(particle.color.withAlpha(alpha));
    
    float size = particle.size * (0.5f + particle.life * 0.5f);
    g.fillEllipse(particle.position.x - size * 0.5f,
                 particle.position.y - size * 0.5f,
                 size, size);
}

void ParticleFieldVisualizer::drawConnections(juce::Graphics& g)
{
    // Draw subtle connections between nearby emitters
    g.setColour(juce::Colour(0xff1a1a1a));
    
    for (size_t i = 0; i < emitters.size(); ++i)
    {
        if (!emitters[i].enabled)
            continue;
        
        for (size_t j = i + 1; j < emitters.size(); ++j)
        {
            if (!emitters[j].enabled)
                continue;
            
            float distance = emitters[i].position.getDistanceFrom(emitters[j].position);
            
            // Only draw connections for nearby emitters
            if (distance < 200.0f)
            {
                float alpha = (1.0f - distance / 200.0f) * 0.1f;
                g.setColour(juce::Colour(0xff2a2a2a).withAlpha(alpha));
                g.drawLine(emitters[i].position.x, emitters[i].position.y,
                          emitters[j].position.x, emitters[j].position.y, 1.0f);
            }
        }
    }
}

} // namespace ui
