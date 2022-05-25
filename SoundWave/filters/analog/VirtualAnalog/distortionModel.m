classdef distortionModel < DKmodel
    
    properties(Constant)
        components_def = [inputPort('In',[1,2],0),...
            resistor('R1',[2,3],10000),...
            resistor('R2',[4,0],1000000),...
            resistor('R3',[6,7],10000),...
            resistor('R4',[7,0],10000),...
            resistor('R5',[5,8],1000000),...
            resistor('R6',[8,9],1000000),...
            resistor('R7',[9,10],4700),...
            capacitor('C1',[1,0],0.001e-6),...
            capacitor('C2',[3,4],0.01e-6),...   
            capacitor('C3',[5,6],1e-6),... 
            capacitor('C4',[7,0],1e-6),...
            capacitor('C5',[10,0],0.047e-6),...
            opa('opa1',[4,8,5],1e8),...
            outputPort('Out',[7,0])];
        
        components_count = struct('numResistors', 7,'numCapacitors', 5,...
            'numInputPorts', 1, 'numOutputPorts', 1,...
            'numNonlinearComponents',0, 'numPotmeters', 0,...
            'numNodes', 11, 'numOPAs', 1);
    end
    
    properties
        nonlin_model;
    end
    
    methods
        function obj = distortionModel(fs)
            obj.T = 1/fs;
            %obj.nonlin_model = obj.components_def(13).model;
            obj = buildModel(obj, obj.components_def,obj.components_count);
        end
        
        function obj = load_input(obj,in)
            obj.U(1) = in; % loads the input signal sample given by in into inputs vector U
        end
        
        function [i, J] = nonlinearity(obj,v)
            [i,J] = obj.nonlin_model(v);
            %[i, J] = ecc83_tube_model(v);
        end
        
    end
end