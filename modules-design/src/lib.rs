use std::ffi::c_double;

use num::traits::Pow;

const PI: f64 = 3.14159265358979323846;
const FREQ_C4: f64 = 261.6;

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

#[no_mangle]
pub unsafe extern "C" fn tftp_freq(current: c_double, increment: c_double) -> c_double {
    let pitch: f64 = num::clamp(current + increment, -4.0, 4.0);
    pitch.powf(2.0) * FREQ_C4
}

#[no_mangle]
pub unsafe extern "C" fn tftp_brightness(phase: c_double) -> c_double {
    if phase < 0.5 {
        return 1.0;
    }
    0.0
}

#[no_mangle]
pub unsafe extern "C" fn tftp_sin(phase: c_double) -> c_double {
    let val = 2.0 * PI * phase;
    val.sin()
}
