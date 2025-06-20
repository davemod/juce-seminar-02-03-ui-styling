#pragma once

#include <JuceHeader.h>

class FilmstripSlider : public juce::Slider
{
public:
	FilmstripSlider ()
	{
		setFilmstrip (juce::ImageFileFormat::loadFrom (BinaryData::knob1_png, BinaryData::knob1_pngSize), 128);
	}

	void setFilmstrip (const juce::Image& newFilmstrip, int newNumImages = -1)
	{
		filmstrip = newFilmstrip;
		numImages = newNumImages;

		if (filmstrip.isValid ())
		{
			jassert (numImages > 0);
			
			if (numImages > 0)
			{
				// store width, height of a single image and orientation of the filmstrip
				isHorizontal = filmstrip.getWidth () > filmstrip.getHeight ();
				
				if (isHorizontal)
				{
					singleImageWidth = filmstrip.getWidth () / numImages;
					singleImageHeight = filmstrip.getHeight ();
				}
				else
				{
					singleImageWidth = filmstrip.getWidth ();
					singleImageHeight = filmstrip.getHeight () / numImages;
				}
			}
		}

		repaint ();
	}

	void paint (juce::Graphics& g) override
	{
		// paint slider if filmstrip is invalid
		if (filmstrip.isNull())
			return juce::Slider::paint (g);

		// get the normalised slider value by using its normalisable range
		auto normalizedValue = getNormalisableRange().convertTo0to1 (getValue ());

		// calculate the index of the image to draw
		auto index = roundToInt (normalizedValue * (numImages - 1));
		jassert (index >= 0 && index < numImages);

		// get the clipped image
		auto clippedImage = filmstrip.getClippedImage (juce::Rectangle<int> (
			isHorizontal ? index * singleImageWidth : 0,
			isHorizontal ? 0 : index * singleImageHeight,
			singleImageWidth,
			singleImageHeight
		));

		// get the layout of the slider from current look and feel
		juce::Slider::SliderLayout layout = getLookAndFeel().getSliderLayout (*this);

		// draw the clipped image at the slider bounds, centred
		g.drawImage (
			clippedImage,
			layout.sliderBounds.toFloat(),
			juce::RectanglePlacement::centred
		);
	}

private:
	juce::Image filmstrip;
	int numImages{ 0 };
	bool isHorizontal{ false };
	int singleImageWidth{ 0 };
	int singleImageHeight{ 0 };

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FilmstripSlider)
};