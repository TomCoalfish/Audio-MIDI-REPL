require('luapa')
require('luastk')
require('luapm')
require('autowah')


luastk.Stk.setRawwavePath( "stk/rawwaves/" );
osc = luastk.BeeThree()
adsr = luastk.ADSR()
adsr:setAllTimes(0.02,0.1,0.75,0.2)
last_note = 0
awah = autowah.Autowah_New(44100)

function noise(input,output,frames)
    luapm.LockMidi()
    for i = 0,2*frames-1,2 do
        local x = osc:tick() * adsr:tick()       
        x = autowah.Autowah_Tick(awah,x)
        luapa.float_set(output,i,x)
        luapa.float_set(output,i+1,x)
    end
    luapm.UnlockMidi()
end 
function freq_to_midi(f)
    return 12.0*math.log(f/440.0)/math.log(2) + 69
end 
function midi_to_freq(m)
    return math.pow(2.0, (m-69)/12)*440.0
end
function note_on(c,n,v)    
    local f = math.pow(2.0, (n-69)/12)*440.0    
    while(luapa.isAudioRunning == true) do end
    osc:noteOn(f,1.0)
    adsr:keyOn()    
    last_note = n
end
function note_off(c,n,v)        
    while(luapa.isAudioRunning == true) do end
    if(n == last_note) then adsr:keyOff() end    
end
function callback()
    
end 
luapm.set_note_on_func(note_on)
luapm.set_note_off_func(note_off)
luapm.InitMidi(1,3,3)
luapa.set_audio_func(noise)
--luapa.set_callback_func(callback)
luapa.Pa_Initialize()
for i=0,luapa.GetNumDevices()-1 do 
    print(i,luapa.GetDeviceName(i))
end
-- use jack on my system it is device 6
-- no input is used (-1)
luapa.InitAudioDevice(14,-1,2,44100,128)
luapa.RunAudio()
luapa.StopAudio()