import serial, csv, time, sys

PORT    = "COM5"
BAUD    = 115200
OUTFILE = "flight_log.csv"
COLS    = ["roll","pitch","roll_rate","pitch_rate","throttle","pid_roll","pid_pitch"]

def collect(secs=600):
    print(f"Logging {secs}s to {OUTFILE}")
    with serial.Serial(PORT,BAUD,timeout=1) as ser, \
         open(OUTFILE,"w",newline="") as f:
        w=csv.writer(f); w.writerow(COLS)
        t0=time.time(); n=0
        while time.time()-t0<secs:
            line=ser.readline().decode("ascii",errors="ignore").strip()
            if not line: continue
            parts=line.split(",")
            if len(parts)!=7: continue
            try:
                w.writerow([float(p) for p in parts]); n+=1
                if n%500==0: print(f"  {n} samples",end="\r")
            except ValueError: continue
    print(f"\nDone: {n} samples")

if __name__=="__main__":
    collect(int(sys.argv[1]) if len(sys.argv)>1 else 600)