package com.maxeler.examples.myproject;

import com.maxeler.maxcompiler.v2.build.EngineParameters;
import com.maxeler.maxcompiler.v2.managers.DFEModel;

public class PassThroughEngineParams extends EngineParameters {

	public static String s_numberOfBits = "numberOfBits";
	public static String s_tag = "tag";

	public PassThroughEngineParams(String[] args) {
		super (args);
	}

	@Override protected void declarations() {
		declareParam(s_parameterDFE, DataType.DFEMODEL, DFEModel.MAIA);
		declareParam(s_parameterMaxFileName, DataType.STRING, "PassThrough");
		declareParam(s_parameterTarget, Target.class, Target.DFE_SIM);

		declareParam(s_tag, DataType.STRING, "");
		declareParam(s_numberOfBits, DataType.INT, 32);
	}

	public int getNumberOfBits() {
		return getParam(s_numberOfBits);
	}

	@Override
	public String getBuildName() {

		String name = super.getBuildName();

		String tag = getParam(s_tag);
		if (tag != "") name += "_"+tag;

		name += "_"+getNumberOfBits()+"bits";

		return name;
	}



}
