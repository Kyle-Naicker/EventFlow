/**
 * @brief build and run a coherent event simulation for TechConnect, 
 *        a technology conference. 
 * 
 * Demonstrates: construction of the Composite; observer registration; 
 * at least three different notices; at least one cascading notification; 
 * a registration change; aComposite traversal/query; a runtime reorganisation; 
 * and clean shutdown.
 *
 * Memory management note: this project does not use the memory or
 * algorithm standard headers, so ownership is entirely explicit. Every EventComponent created with
 * `new` is add()ed to exactly one owning EventGroup, which will later
 * `delete` it (directly or transitively) in its destructor. The only
 * pointer this file itself ever `delete`s directly is `root`.
 */
#include <iostream>
#include "EventGroup.h"
#include "ConcreteLeaves.h"
#include "EventControl.h"

/**
 * @brief Testing all code implemented in the EventFlow project.
 * 
 * @return 0 on successful completion.
 */
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


    control.issueNotice(Notice(NoticeType::OPEN, "Doors open for registration"));

    control.issueNotice(Notice(NoticeType::SCHEDULE_CHANGE, "Keynote moved to 09:30"));

    int threshold = 400;
    int current = root->getCapacity();
    if (current >= threshold) {
        control.issueNotice(Notice(NoticeType::CAPACITY_ALERT,
            "Conference-wide headcount at or above threshold", threshold));
    } else {
        std::cout << "\n(Capacity " << current << " below threshold " << threshold
                  << " -- no CAPACITY_ALERT issued)\n";
    }

    control.issueNotice(Notice(NoticeType::PAUSE, "Severe weather warning issued"));
    control.issueNotice(Notice(NoticeType::RESUME, "Weather warning lifted"));


    control.issueNotice(Notice(NoticeType::VENUE_CHANGE, "Shuttle loop moved to Gate 5 due to roadworks"));

    control.issueNotice(Notice(NoticeType::POWER_OUTAGE, "Loadshedding stage 4 -- switching to generator"));
    std::cout << "(...simulating a 5 minute wait while the generator starts...)\n";
    control.issueNotice(Notice(NoticeType::RESUME, "Generator online, full power restored"));

    
    std::cout << "\n--- Runtime reorganisation: relocating Booth: CloudNova ---\n";
    std::cout << "Before: Sponsor Zone has " << sponsorZone->childCount()
              << " children, Exhibition Wing has " << exhibitionWing->childCount() << " children.\n";
    EventComponent* relocatedBooth = sponsorZone->removeChild(boothA);
    if (relocatedBooth) {
        exhibitionWing->add(relocatedBooth);
        std::cout << "Moved 'Booth: CloudNova' from Sponsor Zone directly under Exhibition Wing.\n";
    }
    std::cout << "After:  Sponsor Zone has " << sponsorZone->childCount()
              << " children, Exhibition Wing has " << exhibitionWing->childCount() << " children.\n";


    std::cout << "\n--- Registration change: Medical Post 1 now also observes EventControl directly ---\n";
    control.attach(medic);
    control.issueNotice(Notice(NoticeType::EVACUATE, "Evacuation drill: south exit"));
    std::cout << "Medical Post 1 received this notice twice: once cascaded via its zone, "
                 "once directly from EventControl -- demonstrating attach() allows an "
                 "observer to register with more than one subject.\n";
    control.detach(medic);


    control.issueNotice(Notice(NoticeType::CANCEL, "Remainder of the day's programme cancelled"));

    std::cout << "\n--- Final structure before shutdown ---\n";
    root->reportStatus();

    std::cout << "\n--- Clean shutdown: destroying root ---\n";
    root->close();
    control.detach(root);
    delete root;

    std::cout << "\nSimulation complete. No leaks: every EventComponent was owned by "
                 "exactly one EventGroup at a time, all reachable from 'root'.\n";


    return 0;
}