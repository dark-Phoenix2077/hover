import numpy as np, pandas as pd
import tensorflow as tf
from sklearn.model_selection import train_test_split
import json

df = pd.read_csv("flight_log.csv").dropna()
X  = df[["roll","pitch","roll_rate","pitch_rate","throttle"]].values.astype("float32")
y  = df[["pid_roll","pid_pitch"]].values.astype("float32")

# Normalise — save stats for firmware dequantisation
Xm,Xs = X.mean(0),X.std(0); ym,ys = y.mean(0),y.std(0)
X = (X-Xm)/Xs;  y = (y-ym)/ys

json.dump({"Xm":Xm.tolist(),"Xs":Xs.tolist(),
           "ym":ym.tolist(),"ys":ys.tolist()},
          open("norm_stats.json","w"))

Xtr,Xva,ytr,yva = train_test_split(X,y,test_size=0.2,random_state=42)

model = tf.keras.Sequential([
    tf.keras.layers.Dense(16,activation="relu",input_shape=(5,)),
    tf.keras.layers.Dense(16,activation="relu"),
    tf.keras.layers.Dense(2)
])
model.compile(optimizer="adam",loss="mse",metrics=["mae"])
model.fit(Xtr,ytr,epochs=50,batch_size=256,
          validation_data=(Xva,yva),verbose=1)
model.save("model/stabilizer_keras")
print("Done.")