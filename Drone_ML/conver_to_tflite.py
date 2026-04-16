import tensorflow as tf, numpy as np, pandas as pd

model = tf.keras.models.load_model("model/stabilizer_keras")
df    = pd.read_csv("flight_log.csv").dropna()
X     = df[["roll","pitch","roll_rate","pitch_rate","throttle"]].values.astype("float32")

def rep_data():
    for i in range(min(500,len(X))):
        yield [X[i:i+1]]

conv = tf.lite.TFLiteConverter.from_keras_model(model)
conv.optimizations = [tf.lite.Optimize.DEFAULT]
conv.representative_dataset = rep_data
conv.target_spec.supported_ops = [tf.lite.OpsSet.TFLITE_BUILTINS_INT8]
conv.inference_input_type  = tf.int8
conv.inference_output_type = tf.int8

tflite = conv.convert()
open("model/stabilizer.tflite","wb").write(tflite)
print(f"Model size: {len(tflite)/1024:.1f} KB")

# Generate C array for STM32
import subprocess
subprocess.run(["xxd","-i","model/stabilizer.tflite",
                "FC_Firmware/Core/Src/stabilizer_model.c"])
print("C array written to FC_Firmware/Core/Src/stabilizer_model.c")