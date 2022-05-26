%{
#include "Tonic.h"
%}
namespace Tonic 
{

  //! Abstract class to be extended by any class that wants to be notified of control events. 
  class ControlChangeSubscriber{
    public:
    virtual ~ControlChangeSubscriber();
    virtual void valueChanged(string, TonicFloat)=0;
  };
  
  namespace Tonic_ 
  {

    class ControlChangeNotifier_ : public ControlConditioner_
    {
    public:
      ControlChangeNotifier_();
      ~ControlChangeNotifier_();
      void addValueChangedSubscriber(ControlChangeSubscriber* sub);
      void removeValueChangedSubscriber(ControlChangeSubscriber* sub);
      void sendControlChangesToSubscribers();
      string name;
    };
    
  }
  
  class ControlChangeNotifier : public TemplatedControlConditioner<Tonic::ControlChangeNotifier, Tonic_::ControlChangeNotifier_>
  {  
  public:
  
    void sendControlChangesToSubscribers();
    void addValueChangedSubscriber(ControlChangeSubscriber* resp){gen()->addValueChangedSubscriber(resp);};
    void removeValueChangedSubscriber(ControlChangeSubscriber* sub){gen()->removeValueChangedSubscriber(sub);};
    void setName(string name){gen()->name = name;}
  };
}

#endif