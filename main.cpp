/**
 * @brief build and run a coherent event simulation for TechConnect, 
 *        a technology conference. 
 * 
 * Demonstrates construction of the Composite, observer registration,
 * at least three different notices, at least one cascading notification,
 * a registration change, a Composite traversal/query, a runtime reorganisation,
 * and clean shutdown.
 *
 * Every EventComponent created with new is add()ed to exactly one owning 
 * EventGroup, which will later delete it (directly or transitively) in its 
 * destructor. The only pointer this file itself ever deletes directly is root.
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

    // Construction of the composite tree (>= 3 levels of nesting and at least 5 concrete leaves)
    //    TechConnect (root EventGroup)
    //      -> Innovation Wing (EventGroup)          [level 1]
    //           -> Keynote Zone (EventGroup)        [level 2]
    //                -> LectureHall (leaf)          [level 3]
    //                -> MedicalStation (leaf)
    //           -> Workshop Zone (EventGroup)       [level 2]
    //                -> WorkshopRoom (leaf)
    //                -> RegistrationDesk (leaf)
    //      -> Exhibition Wing (EventGroup)          [level 1]
    //           -> Sponsor Zone (EventGroup)        [level 2]
    //                -> ExhibitionBooth (leaf)x2
    //                -> CateringStation (leaf)
    //           -> ShuttleStop (leaf, directly under wing)
    //
    // Every new below is add()ed exactly once to an owning EventGroup;
    // the raw pointers kept alongside (keynoteZone, medic, etc.) are
    // non-owning handles used later in the simulation; their actual
    // owner is whichever EventGroup they were add()ed to.
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

    // Observer registration at the root/client boundary: EventControl
    // is the top-level Subject; root observes it. (Lower-level
    // Observer registrations already happened automatically inside
    // each EventGroup::add() call above.)
    EventControl control("TechConnect 2026");
    control.attach(root);

    std::cout << "--- Initial structure (Composite traversal / query) ---\n";
    root->reportStatus();
    std::cout << "Aggregate conference capacity: " << root->getCapacity() << "\n";

    // Recursive Composite operation, distinct from any Observer notice.
    root->open();

    // At least three different notices, including a cascade through
    // multiple runtime levels (EventControl -> root -> wing -> zone ->
    // leaf is 4 hops here).
    control.issueNotice(Notice(NoticeType::OPEN, "Doors open for registration"));

    control.issueNotice(Notice(NoticeType::SCHEDULE_CHANGE, "Keynote moved to 09:30"));

    // Condition-based decision (Task 4.3): capacity threshold check performed 
    // by the client/control layer before deciding to issue a CAPACITY_ALERT
    int threshold = 400;
    int current = root->getCapacity();
    if (current >= threshold) {
        control.issueNotice(Notice(NoticeType::CAPACITY_ALERT,
            "Conference-wide headcount at or above threshold", threshold));
    } else {
        std::cout << "\n(Capacity " << current << " below threshold " << threshold
                  << " -- no CAPACITY_ALERT issued)\n";
    }

    // Weather/safety-style PAUSE cascading through 3+ runtime levels,
    // with different leaves reacting differently through polymorphism.
    control.issueNotice(Notice(NoticeType::PAUSE, "Severe weather warning issued"));
    control.issueNotice(Notice(NoticeType::RESUME, "Weather warning lifted"));

    // Original Features (Task 4.4)
    control.issueNotice(Notice(NoticeType::VENUE_CHANGE, "Shuttle loop moved to Gate 5 due to roadworks"));

    // POWER_OUTAGE: pause everything, then simulate the generator kicking
    // in ~5 minutes later by issuing RESUME. (No real-time sleep is used
    // in this automated demo; the RESUME below stands in for "5
    // minutes later" as documented in the design rationale.)
    control.issueNotice(Notice(NoticeType::POWER_OUTAGE, "Loadshedding stage 4 -- switching to generator"));
    std::cout << "(...simulating a 5 minute wait while the generator starts...)\n";
    control.issueNotice(Notice(NoticeType::RESUME, "Generator online, full power restored"));

    // Runtime reorganisation: move the Booth "CloudNova" from the
    // Sponsor Zone directly to the Exhibition Wing. Both Composite
    // ownership and Observer registration must move together.
    std::cout << "\n--- Runtime reorganisation: relocating Booth: CloudNova ---\n";
    std::cout << "Before: Sponsor Zone has " << sponsorZone->childCount()
              << " children, Exhibition Wing has " << exhibitionWing->childCount() << " children.\n";
    // removeChild() releases ownership from sponsorZone and detaches
    // boothA from sponsorZone's observer list in one call; add() on the
    // new parent then re-establishes both ownership and observer
    // registration. The two relationships move together atomically from
    // the caller's point of view, satisfying Task 4.2. If the pointer
    // returned here were instead discarded without being add()ed
    // elsewhere, the caller would be responsible for `delete`ing it to
    // avoid a leak; removeChild() hands over real ownership, not just a view.
    EventComponent* relocatedBooth = sponsorZone->removeChild(boothA);
    if (relocatedBooth) {
        exhibitionWing->add(relocatedBooth);
        std::cout << "Moved 'Booth: CloudNova' from Sponsor Zone directly under Exhibition Wing.\n";
    }
    std::cout << "After:  Sponsor Zone has " << sponsorZone->childCount()
              << " children, Exhibition Wing has " << exhibitionWing->childCount() << " children.\n";

    // Registration change at runtime: attach the medical station to
    // EventControl directly as well as its zone, demonstrating
    // attach/detach independent of Composite ownership, and that an
    // observer can register with more than one subject.
    std::cout << "\n--- Registration change: Medical Post 1 now also observes EventControl directly ---\n";
    control.attach(medic);
    control.issueNotice(Notice(NoticeType::EVACUATE, "Evacuation drill: south exit"));
    std::cout << "Medical Post 1 received this notice twice: once cascaded via its zone, "
                 "once directly from EventControl -- demonstrating attach() allows an "
                 "observer to register with more than one subject.\n";
    control.detach(medic);

    // CANCEL: another original feature, then clean shutdown.
    control.issueNotice(Notice(NoticeType::CANCEL, "Remainder of the day's programme cancelled"));

    std::cout << "\n--- Final structure before shutdown ---\n";
    root->reportStatus();

    std::cout << "\n--- Clean shutdown: destroying root ---\n";
    root->close();
    control.detach(root);
    delete root; // Explicitly destroys the entire owned subtree exactly once.

    std::cout << "\nSimulation complete. No leaks: every EventComponent was owned by "
                 "exactly one EventGroup at a time, all reachable from 'root'.\n";


    return 0;
}