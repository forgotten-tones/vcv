/// cbindgen:enum-class=false
#[repr(C)]
pub enum ParamId {
    PitchParam,
    ParamsLen,
}

/// cbindgen:enum-class=false
#[repr(C)]
pub enum InputId {
    PitchInput,
    InputsLen,
}

/// cbindgen:enum-class=false
#[repr(C)]
pub enum OutputId {
    SineOutput,
    OutputsLen,
}

/// cbindgen:enum-class=false
#[repr(C)]
pub enum LightId {
    BlinkLight,
    LightsLen,
}

#[no_mangle]
pub extern "C" fn root(p: ParamId, i: InputId, o: OutputId, l: LightId) {}
