#ifndef ActionInitialization_h
#define ActionInitialization_h 1

#include "DetectorConstruction.hh"
#include "DarkMatter.hh"

#include "G4VUserActionInitialization.hh"

class ActionInitialization : public G4VUserActionInitialization
{
  public:
    ActionInitialization();
    ActionInitialization(DetectorConstruction* detectorConstruction, DarkMatter* darkMatter);
    virtual ~ActionInitialization();

    virtual void BuildForMaster() const;
    virtual void Build() const;

  private:
    DetectorConstruction* fDetectorConstruction;
    DarkMatter* fDarkMatter;
};

#endif
