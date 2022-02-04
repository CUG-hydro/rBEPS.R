# 输入数据

## Hourly Meteorological forcing

- `jday`: day of year, `-`
- `hour`: (optional), 
- `rad` : `W m-2`
- `tem` : `degC`
- `hum` : `%`
- `pre` : `mm`
- `wind`: `m/s`

## LAI

```R
120.5  30.5  25  0.85  8  2.2  0.4115  0.0      
long., lat., LC, CI, soiltxt, soiltemp, soilwater,snow-dp
```

## clumping index

## soil type

```R
1, sand
2, loamy sand
3, sandy loam
4, loam
5, silty loam
6, sandy clay loam
7, clay loam
8, silty clay loam
9, sandy clay
10, silty clay
11, clay
-999, -
```

### Soil parameters
`nlayers = 5`
* `b`            : Cambell parameter b, `[-]`
* `Ksat`         : saturated hydraulic conductivity, `[m s-1]`
* `fei`          : porosity      , `[m3 m-3]`
* `theta_vfc`    : field capacity, `[m3 m-3]`
* `theta_vwp`    : wilt point    , `[m3 m-3]`
* `thermal_cond` : thermal conductivity. `[W m-1 K-1]`
* `psi_sat`      : water potential at sat, `[m]`

# 根据预热期(spin period)数据自动获取；

- `soiltemp` : `degC`, 0
- `soilwater`: `m^3 m^-3`, 0.3
- `snowdepth`: `m`,        0
