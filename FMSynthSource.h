#pragma once

#include <cstdint>
#include <LibAudio>
#include "FMSynth/Patch.h"
#include "FMSynth/Voice.h"

namespace Audio {

class FMSynthSource {

    public:
    
        template<std::uint32_t channel = 0>
        static inline FMSynthSource& play(const FMSynth::Patch& patch, std::int8_t notenumber, std::int8_t velocity = 127) {
  static FMSynthSource source;
  source._voice.noteOn(patch, notenumber + 23, velocity);  // Add 23 to convert Pokitto notenumber to midikey
            
            // Check if the last audio buffer has already been filled, and if so, add to it
            std::uint32_t idx = audio_playHead >> 9;
            std::uint32_t last = (idx - 1) & (bufferCount - 1);
            if(audio_state[last]) {
                mix(audio_buffer + last * 512, &source);
            }
            
            Audio::connect(channel, &source._voice, channel == 0 ? copy : mix);
            
            return source;
        }
        
        inline void release() {
            _voice.noteOff();
        }
    
    private:
    
        static void copy(std::uint8_t* buffer, void* ptr) {
            FMSynth::Voice<POK_AUD_FREQ>& voice = *reinterpret_cast<FMSynth::Voice<POK_AUD_FREQ>*>(ptr);
            for(std::uint32_t idx = 0; idx < 512; ++idx) {
                buffer[idx] = voice.update();
            }
        }
        
        static void mix(std::uint8_t* buffer, void* ptr) {
            FMSynth::Voice<POK_AUD_FREQ>& voice = *reinterpret_cast<FMSynth::Voice<POK_AUD_FREQ>*>(ptr);
            for(std::uint32_t idx = 0; idx < 512; ++idx ){
                buffer[idx] = Audio::mix(buffer[idx], voice.update());
            }
        }
        
        FMSynth::Voice<POK_AUD_FREQ> _voice;
};

template<std::uint32_t channel = 0>
inline FMSynthSource& play(const FMSynth::Patch& patch, std::int8_t notenumber, std::int8_t velocity = 127) {
    return FMSynthSource::play<channel>(patch, notenumber, velocity);
}

struct FMPatch {
    constexpr FMPatch() : _patch() {}
    
    template<std::uint32_t channel = 0>
    FMSynthSource& play(std::int8_t notenumber, std::int8_t velocity = 127) const {
        return Audio::play<channel>(_patch, notenumber, velocity);
    }
    
    constexpr std::uint8_t algorithm() const { return _patch.algorithm; }
    constexpr FMPatch& algorithm(std::uint8_t id) { _patch.algorithm = id; return *this; }
    
    constexpr std::uint8_t volume() const { return _patch.volume; }
    constexpr FMPatch& volume(std::uint8_t value) { _patch.volume = value; return *this; }
    
    constexpr std::int8_t feedback() const { return _patch.feedback - 50; }
    constexpr FMPatch& feedback(std::int8_t value) { _patch.feedback = value + 50; return *this; }
    
    constexpr std::uint8_t glide() const { return _patch.glide; }
    constexpr FMPatch& glide(std::uint8_t value) { _patch.glide = value; return *this; }
    
    constexpr std::uint8_t attack() const { return _patch.attack; }
    constexpr FMPatch& attack(std::uint8_t value) { _patch.attack = value; return *this; }
    
    constexpr std::uint8_t decay() const { return _patch.decay; }
    constexpr FMPatch& decay(std::uint8_t value) { _patch.decay = value; return *this; }
    
    constexpr std::uint8_t sustain() const { return _patch.sustain; }
    constexpr FMPatch& sustain(std::uint8_t value) { _patch.sustain = value; return *this; }
    
    constexpr std::uint8_t release() const { return _patch.release; }
    constexpr FMPatch& release(std::uint8_t value) { _patch.release = value; return *this; }
    
    struct LFO {
        std::uint8_t _speed;
        constexpr std::uint8_t speed() const { return _speed; }
        constexpr LFO& speed(std::uint8_t value) { _speed = value; return *this; }
        
        std::uint8_t _attack;
        constexpr std::uint8_t attck() const { return _attack; }
        constexpr LFO& attack(std::uint8_t value) { _attack = value; return *this; }
        
        std::uint8_t _pmd;
        constexpr std::uint8_t pmd() const { return _pmd; }
        constexpr LFO& pmd(std::uint8_t value) { _pmd = value; return *this; }
    };
    constexpr FMPatch& initLFO(const LFO& lfo) {
        _patch.lfo.speed = lfo._speed;
        _patch.lfo.attack = lfo._attack;
        _patch.lfo.pmd = lfo._pmd;
        return *this;
    }
    
    struct Operator {
        std::uint8_t _level = 0;
        constexpr std::uint8_t level() const { return _level; }
        constexpr Operator& level(std::uint8_t value) { _level = value; return *this; }
        
        bool _fixed = false;
        constexpr bool fixed() const { return _fixed; }
        constexpr Operator& fixed(bool value) { _fixed = value; return *this; }
        
        std::uint8_t _coarse = 1;
        constexpr std::uint8_t coarse() const { return _coarse; }
        constexpr Operator& coarse(std::uint8_t value) { _coarse = value; return *this; }
        
        std::uint8_t _fine = 0;
        constexpr std::uint8_t fine() const { return _fine; }
        constexpr Operator& fine(std::uint8_t value) { _fine = value; return *this; }
        
        std::int8_t _detune = 0;
        constexpr std::int8_t detune() const { return _detune; }
        constexpr Operator& detune(std::int8_t value) { _detune = value; return *this; }
        
        std::int8_t _attack = 0;
        constexpr std::uint8_t attack() const { return _attack; }
        constexpr Operator& attack(std::uint8_t value) { _attack = value; return *this; }
        
        std::int8_t _decay = 0;
        constexpr std::uint8_t decay() const { return _decay; }
        constexpr Operator& decay(std::uint8_t value) { _decay = value; return *this; }
        
        std::int8_t _sustain = 100;
        constexpr std::uint8_t sustain() const { return _sustain; }
        constexpr Operator& sustain(std::uint8_t value) { _sustain = value; return *this; }
        
        bool _loop = false;
        constexpr bool loop() const { return _loop; }
        constexpr Operator& loop(bool value) { _loop = value; return *this; }
    };
    constexpr FMPatch& initOperator(std::uint32_t idx, const Operator& op) {
        _patch.op[idx].level = op._level;
        _patch.op[idx].pitch.fixed = op._fixed;
        _patch.op[idx].pitch.coarse = op._coarse;
        _patch.op[idx].pitch.fine = op._fine;
        _patch.op[idx].detune = op._detune + 50;
        _patch.op[idx].attack = op._attack;
        _patch.op[idx].decay = op._decay;
        _patch.op[idx].sustain = op._sustain;
        _patch.op[idx].loop = op._loop;
        return *this;
    }
    
    inline const FMSynth::Patch& patch() const { return _patch; }
    inline FMSynth::Patch& patch() { return _patch; }
    
    FMSynth::Patch _patch;
};

} // namespace Audio
