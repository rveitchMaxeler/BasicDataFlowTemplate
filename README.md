# My Project Title

This is a generic project template. My Project does: X, Y, Z.


## How to build

### Environment
```bash
source utils/config.sh
source utils/maxenv.sh
```


### Bitstream
```bash
cd bitstream
ant sim # for Simulation
ant dfe # for Hardware
```


### Runtime
```bash
cd runtime
cp <maxfile> .
./build.py
```


## How to run
```bash
cd runtime
./build.py run_sim
```

