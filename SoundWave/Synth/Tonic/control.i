%{
#include "Tonic.h"
%}

namespace Tonic{

  namespace Tonic_{
    class ControlMidiToFreq_ : public ControlConditioner_    {  };

  }

  class ControlMidiToFreq : public TemplatedControlConditioner<ControlMidiToFreq, Tonic_::ControlMidiToFreq_> {};

}

namespace Tonic{

  namespace Tonic_{

    class ControlFloor_ : public ControlConditioner_{ };

  }

  class ControlFloor : public TemplatedControlConditioner<ControlFloor, Tonic_::ControlFloor_> {};

}

namespace Tonic {
  
  namespace Tonic_ {

    class ControlDbToLinear_ : public ControlConditioner_{};    
  }
  
  class ControlDbToLinear : public TemplatedControlConditioner<ControlDbToLinear, Tonic_::ControlDbToLinear_>{};
}
