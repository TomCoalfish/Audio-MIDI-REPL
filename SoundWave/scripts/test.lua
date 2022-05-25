require('plot')
wt = require('TWaveTable')
v = wt.double_vector(256)
wt.MakeSined(v,256,220,44100)
plot.plot_x(v,256,"")
while 1 do end