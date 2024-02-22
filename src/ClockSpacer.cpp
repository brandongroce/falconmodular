#include "plugin.hpp"


struct ClockSpacer : Module {
	enum ParamId {
		BPM_PARAM,
		DIVISOR_PARAM,
		ONSWITCH_PARAM,
		PARAMS_LEN
	};
	enum InputId {
		RESET_INPUT,
		INPUTS_LEN
	};
	enum OutputId {
		MASTER_OUTPUT,
		OUT1_OUTPUT,
		OUT2_OUTPUT,
		OUT3_OUTPUT,
		OUT4_OUTPUT,
		OUT5_OUTPUT,
		OUT6_OUTPUT,
		OUT7_OUTPUT,
		OUT8_OUTPUT,
		OUTPUTS_LEN
	};
	enum LightId {
		LIGHTS_LEN
	};


    float bpm = 120.f;
    float masterDiv = 1.f; // Default master divisor
    float phase = 0.f;
	float phase1 = 0.f;
	float phase2 = 0.f;
	float phase3 = 0.f;
	float phase4 = 0.f;
	float phase5 = 0.f;
	float phase6 = 0.f;
	float phase7 = 0.f;
	float phase8 = 0.f;

	ClockSpacer() {
		config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
        configParam(BPM_PARAM, 30.f, 300.f, 120.f, "BPM");
        configParam(DIVISOR_PARAM, 0.f, 2.f, 1.f, "Master Divisor");
		configSwitch(ONSWITCH_PARAM, 0.f, 1.f, 1.f, "Quality", {"Off", "On"});
		configInput(RESET_INPUT, "Reset");
		configOutput(MASTER_OUTPUT, "Master Clock");
		configOutput(OUT1_OUTPUT, "-1");
		configOutput(OUT2_OUTPUT, "-2");
		configOutput(OUT3_OUTPUT, "-3");
		configOutput(OUT4_OUTPUT, "-4");
		configOutput(OUT5_OUTPUT, "-5");
		configOutput(OUT6_OUTPUT, "-6");
		configOutput(OUT7_OUTPUT, "-7");
		configOutput(OUT8_OUTPUT, "-8");
	}

	void process(const ProcessArgs& args) override {
		bpm = params[BPM_PARAM].getValue();
        masterDiv = params[DIVISOR_PARAM].getValue();

		if(params[ONSWITCH_PARAM].getValue() > 0.f) {
			if (inputs[RESET_INPUT].getVoltage() >= 1.f) {
				phase = 0.f;
				phase1 = 0.f;
				phase2 = 0.f;
				phase3 = 0.f;
				phase4 = 0.f;
				phase5 = 0.f;
				phase6 = 0.f;
				phase7 = 0.f;
				phase8 = 0.f;

			}

			float minute = 60.0;

			float deltaPhase = (bpm / minute) * args.sampleTime;
			float deltaPhase1 = ((bpm - masterDiv) / minute) * args.sampleTime;
			float deltaPhase2 = ((bpm - (masterDiv * 2)) / minute) * args.sampleTime;
			float deltaPhase3 = ((bpm - (masterDiv * 3)) / minute) * args.sampleTime;
			float deltaPhase4 = ((bpm - (masterDiv * 4)) / minute) * args.sampleTime;
			float deltaPhase5 = ((bpm - (masterDiv * 5)) / minute) * args.sampleTime;
			float deltaPhase6 = ((bpm - (masterDiv * 6)) / minute) * args.sampleTime;
			float deltaPhase7 = ((bpm - (masterDiv * 7)) / minute) * args.sampleTime;
			float deltaPhase8 = ((bpm - (masterDiv * 8)) / minute) * args.sampleTime;

			phase += deltaPhase;
			phase1 += deltaPhase1;
			phase2 += deltaPhase2;
			phase3 += deltaPhase3;
			phase4 += deltaPhase4;
			phase5 += deltaPhase5;
			phase6 += deltaPhase6;
			phase7 += deltaPhase7;
			phase8 += deltaPhase8;

			if (phase >= 1.f) phase -= 1.f;
			if (phase1 >= 1.f) phase1 -= 1.f;
			if (phase2 >= 1.f) phase2 -= 1.f;
			if (phase3 >= 1.f) phase3 -= 1.f;
			if (phase4 >= 1.f) phase4 -= 1.f;
			if (phase5 >= 1.f) phase5 -= 1.f;
			if (phase6 >= 1.f) phase6 -= 1.f;
			if (phase7 >= 1.f) phase7 -= 1.f;
			if (phase8 >= 1.f) phase8 -= 1.f;

			float gateLength = 0.5;

			outputs[MASTER_OUTPUT].setVoltage((phase < gateLength) ? 10.f : 0.f);
			outputs[OUT1_OUTPUT].setVoltage((phase1 < gateLength) ? 10.f : 0.f);
			outputs[OUT2_OUTPUT].setVoltage((phase2 < gateLength) ? 10.f : 0.f);
			outputs[OUT3_OUTPUT].setVoltage((phase3 < gateLength) ? 10.f : 0.f);
			outputs[OUT4_OUTPUT].setVoltage((phase4 < gateLength) ? 10.f : 0.f);
			outputs[OUT5_OUTPUT].setVoltage((phase5 < gateLength) ? 10.f : 0.f);
			outputs[OUT6_OUTPUT].setVoltage((phase6 < gateLength) ? 10.f : 0.f);
			outputs[OUT7_OUTPUT].setVoltage((phase7 < gateLength) ? 10.f : 0.f);
			outputs[OUT8_OUTPUT].setVoltage((phase8 < gateLength) ? 10.f : 0.f);
		}
	}
};


struct ClockSpacerWidget : ModuleWidget {
	ClockSpacerWidget(ClockSpacer* module) {
		setModule(module);
		setPanel(createPanel(asset::plugin(pluginInstance, "res/ClockSpacer.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(15.24, 21.619)), module, ClockSpacer::BPM_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(15.24, 33.541)), module, ClockSpacer::DIVISOR_PARAM));
		addParam(createParamCentered<rack::componentlibrary::BefacoSwitch>(mm2px(Vec(24.697, 39.502)), module, ClockSpacer::ONSWITCH_PARAM));


		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(15.24, 46.063)), module, ClockSpacer::RESET_INPUT));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(15.24, 58.733)), module, ClockSpacer::MASTER_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(6.456, 75.0)), module, ClockSpacer::OUT1_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(24.432, 75.0)), module, ClockSpacer::OUT2_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(6.456, 87.957)), module, ClockSpacer::OUT3_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(24.432, 87.975)), module, ClockSpacer::OUT4_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(6.456, 100.15)), module, ClockSpacer::OUT5_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(24.432, 100.505)), module, ClockSpacer::OUT6_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(6.456, 113.0)), module, ClockSpacer::OUT7_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(24.432, 113.035)), module, ClockSpacer::OUT8_OUTPUT));
	}
};


Model* modelClockSpacer = createModel<ClockSpacer, ClockSpacerWidget>("ClockSpacer");