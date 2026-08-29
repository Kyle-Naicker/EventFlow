#include <iostream>
#include "EventGroup.h"
#include "ConcreteLeaves.h"
#include "EventControl.h"

int main() {
    std::cout << "############################################\n";
    std::cout << "#   TechConnect -- EventFlow Simulation     #\n";
    std::cout << "############################################\n\n";

    EventGroup* root = new EventGroup("TechConnect Conference");

    EventGroup* keynoteZone  = new EventGroup("Keynote Zone");
    EventGroup* workshopZone = new EventGroup("Workshop Zone");
    EventGroup* innovationWing = new EventGroup("Innovation Wing");

    keynoteZone->add(new LectureHall("Hall A", 300));
    MedicalStation* medic = new MedicalStation("Medical Post 1", 4);
    keynoteZone->add(medic);

    WorkshopRoom* workshopRoomPtr = new WorkshopRoom("Robotics Lab", 40);
    workshopZone->add(workshopRoomPtr);
    workshopZone->add(new RegistrationDesk("North Registration", 0));

    innovationWing->add(keynoteZone);
    innovationWing->add(workshopZone);

    EventGroup* sponsorZone    = new EventGroup("Sponsor Zone");
    EventGroup* exhibitionWing = new EventGroup("Exhibition Wing");

    ExhibitionBooth* boothA = new ExhibitionBooth("Booth: CloudNova", 25);
    sponsorZone->add(boothA);
    sponsorZone->add(new ExhibitionBooth("Booth: QuantumEdge", 25));
    CateringStation* catering = new CateringStation("Atrium Catering", 60);
    sponsorZone->add(catering);

    ShuttleStop* shuttle = new ShuttleStop("Gate 3 Shuttle Stop", 0);
    exhibitionWing->add(sponsorZone);
    exhibitionWing->add(shuttle);

    root->add(innovationWing);
    root->add(exhibitionWing);


    EventControl control("TechConnect 2026");
    control.attach(root);

    std::cout << "--- Initial structure (Composite traversal / query) ---\n";
    root->reportStatus();
    std::cout << "Aggregate conference capacity: " << root->getCapacity() << "\n";

    root->open();


    return 0;
}