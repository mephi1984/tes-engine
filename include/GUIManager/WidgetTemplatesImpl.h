
// This file contains templates for widgets
// This file must be included somewhere in HalibutEngine.h just after ResourceManager is declared

namespace SE
{

template <typename TYPENAME>
void TRadioButton::SmartValueFiller(const std::string& dictionaryName, const std::string& smartValueName, TYPENAME value, TRadioButton* radioButtonToFill)
{
    
    radioButtonToFill->SetChecked(ResourceManager->SmartValueManager.GetImmediateValue<TYPENAME>(dictionaryName, smartValueName) == value);
    radioButtonToFill->OnSetChecked.connect
    (
     [dictionaryName, smartValueName, value](bool checked)
     {
         if (checked)
         {
             if (ResourceManager->SmartValueManager.GetImmediateValue<TYPENAME>(dictionaryName, smartValueName) != value)
             {
                 ResourceManager->SmartValueManager.SetImmediateValue<TYPENAME>(dictionaryName, smartValueName, value);
             }
         }
     }
     );
    
    radioButtonToFill->SetCheckedSlotConnection = ResourceManager->SmartValueManager.GetSignal<TYPENAME>(dictionaryName, smartValueName).connect
    (
     [radioButtonToFill, value](const TYPENAME& newValue)
     {
         radioButtonToFill->SetChecked(value == newValue);
     }
     );
}

} //namespace SE
