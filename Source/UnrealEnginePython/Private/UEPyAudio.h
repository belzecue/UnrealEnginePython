#pragma once



#include "UnrealEnginePython.h"

PyObject *py_ue_queue_audio(ue_PyUObject *self, PyObject * args);
PyObject *py_ue_play_sound_at_location(ue_PyUObject *, PyObject *);
PyObject *py_ue_sound_get_data(ue_PyUObject *self, PyObject * args);