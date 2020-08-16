/*
    This file is part of Helio Workstation.

    Helio is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Helio is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Helio. If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include "Chord.h"
#include "BaseResource.h"

class Scale final : public BaseResource
{
public:

    Scale() = default;
    Scale(const Scale &other) noexcept;

    String getResourceId() const noexcept override;
    Identifier getResourceType() const noexcept override;
    using Ptr = ReferenceCountedObjectPtr<Scale>;

    Scale::Ptr withName(const String &name) const noexcept;
    Scale::Ptr withKeys(const Array<int> &keys) const noexcept;

    // These names only make sense in diatonic scales:
    enum Function
    {
        Tonic = 0,
        Supertonic = 1,
        Mediant = 2,
        Subdominant = 3,
        Dominant = 4,
        Submediant = 5,
        Subtonic = 6
    };
    
    //===------------------------------------------------------------------===//
    // Helpers
    //===------------------------------------------------------------------===//

    bool isValid() const noexcept;
    int getSize() const noexcept;
    String getLocalizedName() const;

    // Render target chord into chromatic scale (tonic = 0)
    Array<int> getChord(const Chord::Ptr chord, Function fun, bool oneOctave) const;

    Array<int> getUpScale() const;
    Array<int> getDownScale() const;

    // Flat third considered "minor"-ish (like Aeolian, Phrygian, Locrian etc.)
    bool seemsMinor() const noexcept;

    // Chromatic key will be wrapped from 0 to scale's period size
    // True if specified chromatic key is within the scale
    bool hasKey(int chormaticKey) const;

    // Chromatic key will be wrapped from 0 to scale's period size
    // Returns -1 if chromatic key is not found within the scale
    int getScaleKey(int chormaticKey) const;

    // Chromatic key will be wrapped from 0 to scale's period size
    // Returns index of the closest in-scale key
    int getNearestScaleKey(int chromaticKey) const;

    // Key (input and returned) starts from 0
    int getChromaticKey(int inScaleKey, int extraChromaticOffset,
        bool restrictToOneOctave) const noexcept;

    // Base octave size - like 12 tone chromatic octave for diatonic scales
    int getBasePeriod() const noexcept;

    //===------------------------------------------------------------------===//
    // Hard-coded defaults
    //===------------------------------------------------------------------===//

    static Scale::Ptr getChromaticScale();
    static Scale::Ptr getNaturalMinorScale();
    static Scale::Ptr getNaturalMajorScale();

    //===------------------------------------------------------------------===//
    // Serializable
    //===------------------------------------------------------------------===//

    SerializedData serialize() const override;
    void deserialize(const SerializedData &data) override;
    void reset() override;

    //===------------------------------------------------------------------===//
    // Operators
    //===------------------------------------------------------------------===//

    Scale &operator=(const Scale &other);
    friend bool operator==(const Scale &l, const Scale &r);
    friend bool operator!=(const Scale &l, const Scale &r);

    // Used to compare scales in version control
    // (there may be lots of synonyms for the same sets of notes,
    // e.g. Phrygian is called Zokuso in Japan and Ousak in Greece)
    bool isEquivalentTo(const Scale *other) const;
    bool isEquivalentTo(const Scale::Ptr other) const;

    int hashCode() const noexcept;

private:

    String name;

    // Simply holds key indices for chromatic scale
    // accessed by index in target scale,
    // e.g. for Ionian: keys[0] = 0, keys[1] = 2, keys[2] = 4, etc
    Array<int> keys;

    int basePeriod = Globals::twelveTonePeriodSize;

private:

    // "anonymous" scales, helpers for temperaments
    static Scale::Ptr fromIntervalsAndPeriod(const String &intervals, int periodSize);
    String getIntervals() const noexcept;
    friend class Temperament;

    JUCE_LEAK_DETECTOR(Scale)
};
