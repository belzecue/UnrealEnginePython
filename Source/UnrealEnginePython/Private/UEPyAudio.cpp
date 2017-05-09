#include "UnrealEnginePythonPrivatePCH.h"
#include "Sound/SoundWaveProcedural.h"

PyObject *py_ue_queue_audio(ue_PyUObject *self, PyObject * args) {
	// Writes from a Python buffer object to a USoundWaveProcedural class
	ue_py_check(self);

	Py_buffer sound_buffer;

	if (!PyArg_ParseTuple(args, "y*:queue_audio", &sound_buffer)) {
		return NULL;
	}

	USoundWaveProcedural *sound_wave_procedural = ue_py_check_type<USoundWaveProcedural>(self);
	if (!sound_wave_procedural)
		return PyErr_Format(PyExc_Exception, "UObject is not a USoundWaveProcedural.");

	// Convert the buffer
	uint8 *buffer = (uint8 *)sound_buffer.buf;
	if (buffer == nullptr)
		return PyErr_Format(PyExc_Exception, "Invalid sound buffer.");

	// Add the audio to the Sound Wave's audio buffer
	sound_wave_procedural->QueueAudio(buffer, sound_buffer.len);

	// Clean up
	PyBuffer_Release(&sound_buffer);

	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *py_ue_sound_get_data(ue_PyUObject *self, PyObject * args) {
	ue_py_check(self);

	USoundWave *sound = ue_py_check_type<USoundWave>(self);
	if (!sound)
		return PyErr_Format(PyExc_Exception, "UObject is not a USoundWave.");

	FByteBulkData raw_data = sound->RawData;

	char *data = (char *)raw_data.Lock(LOCK_READ_ONLY);
	int32 data_size = raw_data.GetBulkDataSize();
	PyObject *py_data = PyBytes_FromStringAndSize(data, data_size);
	raw_data.Unlock();
	return py_data;
}

PyObject *py_ue_play_sound_at_location(ue_PyUObject *self, PyObject * args) {

	ue_py_check(self);

	PyObject *sound;
	PyObject *py_location;
	float volume = 1;
	float pitch = 1;
	float start = 0;

	if (!PyArg_ParseTuple(args, "OO|fff:play_sound_at_location", &sound, &py_location, &volume, &pitch, &start)) {
		return NULL;
	}

	USoundBase *sound_object = nullptr;
	if (ue_is_pyuobject(sound)) {
		sound_object = ue_py_check_type<USoundBase>(sound);
	}
	else if (PyUnicodeOrString_Check(sound)) {
		sound_object = FindObject<USoundBase>(ANY_PACKAGE, UTF8_TO_TCHAR(PyUnicode_AsUTF8(sound)));
	}

	if (!sound_object)
		return PyErr_Format(PyExc_Exception, "invalid sound object");

	ue_PyFVector *location = py_ue_is_fvector(py_location);
	if (!location)
		return PyErr_Format(PyExc_TypeError, "sound location must be a FVector");

	UGameplayStatics::PlaySoundAtLocation(self->ue_object, sound_object, location->vec, volume, pitch, start);

	Py_INCREF(Py_None);
	return Py_None;
}