%{
#include "Tonic.h"
%}

namespace Tonic {

   TonicDictionary<SampleTable> * s_oscillatorTables();
    
   class TableLookupOsc_ : public Generator_
   {
   public:
      
      TableLookupOsc_();

      void reset( void );      
      void setFrequency( Generator genArg);
      void setLookupTable( SampleTable table );

    };
    
  
  class TableLookupOsc : public TemplatedGenerator<Tonic_::TableLookupOsc_>{
    
    public:
    
      TableLookupOsc & setLookupTable( SampleTable lookupTable );    
      TableLookupOsc& freq(Generator arg;
  };
