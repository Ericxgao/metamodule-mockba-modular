// Moog style Square .17 oscillator by Mockba the Borg

#include "plugin.hpp"
#include "MockbaModular.hpp"

struct MaugSquare3 : Module {
	enum ParamIds {
		_LFO_PARAM,
		_FREQ_PARAM,
		_FINE_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
		_MODF_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		_WAVE_OUTPUT,
		NUM_OUTPUTS
	};
	enum LightIds {
		NUM_LIGHTS
	};

	_MaugOsc osc[4];

	MaugSquare3() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
		configParam<_OnOff>(_LFO_PARAM, 0, 1, 0, "");
		configParam(_FREQ_PARAM, -54.f, 54.f, 0.f, "Frequency", " Hz", dsp::FREQ_SEMITONE, dsp::FREQ_C4);
		configParam(_FINE_PARAM, -1.f, 1.f, 0.f, "Fine frequency / LFO Offset");
	}

	void onAdd() override;

	void onReset() override;

	void process(const ProcessArgs& args) override;
};

void MaugSquare3::onAdd() {
	osc->init();
}

void MaugSquare3::onReset() {
	onAdd();
}

void MaugSquare3::process(const ProcessArgs& args) {
	// Get the frequency parameters
	float freqParam = params[_FREQ_PARAM].getValue() / 12.f;
	// LFO mode
	if (params[_LFO_PARAM].getValue())
		freqParam = (freqParam * 2) - 5;
	freqParam += dsp::quadraticBipolar(params[_FINE_PARAM].getValue()) * 3.f / 12.f;
	// Iterate over each channel
	int channels = max(inputs[_MODF_INPUT].getChannels(), 1);
	for (int c = 0; c < channels; c += 4) {
		// Get the oscillator
		auto* oscillator = &osc[c / 4];
		oscillator->channels = min(channels - c, 4);
		float_4 pitch = freqParam;
		// Set the pitch
		pitch += inputs[_MODF_INPUT].getVoltageSimd<float_4>(c);
		oscillator->setWave(5.f);
		oscillator->setPitch(pitch, 1.f);
		// Process and output
		oscillator->process(args.sampleTime);
		float_4 off = params[_LFO_PARAM].getValue() * params[_FINE_PARAM].getValue() * 5.f;
		outputs[_WAVE_OUTPUT].setVoltageSimd(5.f * oscillator->_Out() + off, c);
	}
	outputs[_WAVE_OUTPUT].setChannels(channels);
}

struct MaugSquare3Widget : ModuleWidget {
	MaugSquare3Widget(MaugSquare3* module) {
		setModule(module);
		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/MaugSquare3.svg")));


		// Screws
		addChild(createWidget<_Screw>(Vec(0, 0)));
		addChild(createWidget<_Screw>(Vec(box.size.x - RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		// Knobs
		addParam(createParamCentered<_Hsw>(mm2px(Vec(5.1, 68.0)), module, MaugSquare3::_LFO_PARAM));
		addParam(createParamCentered<_Knob>(mm2px(Vec(5.1, 79.0)), module, MaugSquare3::_FREQ_PARAM));
		addParam(createParamCentered<_Knob>(mm2px(Vec(5.1, 90.0)), module, MaugSquare3::_FINE_PARAM));

		// Inputs
		addInput(createInputCentered<_Port>(mm2px(Vec(5.1, 101.0)), module, MaugSquare3::_MODF_INPUT));

		// Outputs
		addOutput(createOutputCentered<_Port>(mm2px(Vec(5.1, 112.0)), module, MaugSquare3::_WAVE_OUTPUT));
	}
};

Model* modelMaugSquare3 = createModel<MaugSquare3, MaugSquare3Widget>("MaugSquare3");