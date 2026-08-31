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

    // SD1 - Building and registering part of the event.
    // Show the client creating a root Composite, at least one nested
    // Composite and at least two Leaves, adding them to the ownership
    // tree, and establishing the Observer registrations needed for
    // later notifications: root, Keynote Zone, and two LectureHalls.
    // Composite ownership (add()) and Observer registration (the
    // attach() that add() performs internally) are both exercised here,
    // and are distinguishable in the resulting sequence diagram because
    // add() itself calls attach() as a nested, separately-labelled call.
    EventGroup* root = new EventGroup("TechConnect Conference", "09:00", "17:00");
    EventGroup* keynoteZone = new EventGroup("Keynote Zone", "10:00", "16:00");

    LectureHall* beyondTheAlgorithm = new LectureHall(
        "Beyond the Algorithm", 300, /*indoors=*/true, "10:00", "12:00");
    CateringStation* coffeeStation = new CateringStation(
        "Coffee Station", 40, /*indoors=*/false, "10:00", "16:00");
    

    keynoteZone->add(beyondTheAlgorithm);  // ownership + auto-attach as observer
    keynoteZone->add(coffeeStation);       // ownership + auto-attach as observer
    root->add(keynoteZone);                // ownership + auto-attach as observer

    // The complete tree, continued: the rest of Keynote Zone, then
    // Workshop Zone, Sponsor Zone, and every leaf attached directly to
    // the root, matching the required structure exactly:
    //
    //     TechConnect (09:00-17:00)                                [level 1]
    //       -> RegistrationDesk "Main Desk" (09:00-17:00)          [level 2]
    //       -> Keynote Zone (10:00-16:00)                          [level 2]
    //            -> LectureHall "Beyond the Algorithm" (10:00-12:00) [level 3]
    //            -> LectureHall "Quantum Leaps" (13:00-15:00)
    //            -> LectureHall "Bridging the Divide" (16:00-17:00) [outdoor]
    //            -> CateringStation "Coffee Station" (10:00-16:00)  [outdoor]
    //       -> Workshop Zone (13:00-17:00)                         [level 2]
    //            -> WorkshopRoom "Robotics Lab" (13:00-15:00)      [level 3]
    //            -> WorkshopRoom "VR Lab" (15:00-17:00)
    //       -> Sponsor Zone (09:00-17:00)                          [level 2]
    //            -> ExhibitionBooth "CloudNova" (09:00-17:00)      [level 3]
    //            -> ExhibitionBooth "QuantumEdge" (09:00-17:00)
    //       -> MedicalStation "First Aid" (09:00-17:00)            [level 2]
    //       -> CateringStation "Radisson Blu Catering" (09:00-17:00) [level 2]
    //       -> ShuttleStop "Gate 3 Shuttle Stop" (09:00-17:00, always outdoor) [level 2]
    LectureHall* quantumLeaps = new LectureHall(
        "Quantum Leaps", 300, /*indoors=*/true, "13:00", "15:00");
    LectureHall* bridgingTheDivide = new LectureHall(
        "Bridging the Divide", 150, /*indoors=*/false, "16:00", "17:00");
    keynoteZone->add(quantumLeaps);
    keynoteZone->add(bridgingTheDivide);

    EventGroup* workshopZone = new EventGroup("Workshop Zone", "13:00", "17:00");
    WorkshopRoom* roboticsLab = new WorkshopRoom(
        "Robotics Lab", 40, /*indoors=*/true, "13:00", "15:00");
    WorkshopRoom* vrLab = new WorkshopRoom(
        "VR Lab", 30, /*indoors=*/true, "15:00", "17:00");
    workshopZone->add(roboticsLab);
    workshopZone->add(vrLab);

    EventGroup* sponsorZone = new EventGroup("Sponsor Zone", "09:00", "17:00");
    ExhibitionBooth* cloudNova = new ExhibitionBooth(
        "CloudNova", 25, /*indoors=*/true, "09:00", "17:00");
    ExhibitionBooth* quantumEdge = new ExhibitionBooth(
        "QuantumEdge", 25, /*indoors=*/true, "09:00", "17:00");
    sponsorZone->add(cloudNova);
    sponsorZone->add(quantumEdge);

    RegistrationDesk* mainDesk = new RegistrationDesk(
        "Main Desk", 0, /*indoors=*/true, "09:00", "17:00");
    MedicalStation* firstAid = new MedicalStation(
        "First Aid", 4, /*indoors=*/true, "09:00", "17:00");
    CateringStation* radissonBlu = new CateringStation(
        "Radisson Blu Catering", 60, /*indoors=*/true, "09:00", "17:00");
    ShuttleStop* gate3Shuttle = new ShuttleStop(
        "Gate 3 Shuttle Stop", 0, "09:00", "17:00");

    root->add(mainDesk);
    root->add(workshopZone);
    root->add(sponsorZone);
    root->add(firstAid);
    root->add(radissonBlu);
    root->add(gate3Shuttle);

    // EventControl is the root/client-boundary Subject; root observes it.
    EventControl control("TechConnect 2026");
    control.attach(root);

    std::cout << "--- Initial structure (Composite traversal / query) ---\n";
    root->reportStatus();
    std::cout << "Aggregate conference capacity: " << root->getCapacity() << "\n";

    // Structural open() call: recursive Composite operation, distinct
    // from any Observer notice.
    root->open();

    // A first, simple cascaded notice: OPEN affects every EventComponent
    // (Task rule), so every leaf and every group reacts as the notice
    // cascades from EventControl through root, through each zone, down
    // to every leaf - easily the deepest and widest cascade in the
    // simulation, good for illustrating loop-fragment behaviour.
    control.issueNotice(Notice(NoticeType::OPEN, "Doors open, welcome to TechConnect"));

    // SD2 - Cascading event notification.
    // A system-wide WEATHER_ALERT, cascaded from the root, that only
    // affects outdoor ConcreteLeaves. At least two concrete units react
    // meaningfully here: Gate 3 Shuttle Stop (always outdoors) and
    // Bridging the Divide (an outdoor LectureHall) both react; Coffee
    // Station (an outdoor CateringStation) also reacts. Every indoor
    // leaf and every EventGroup receives the same notice via the same
    // cascade but has no case for it, so nothing is printed for them -
    // demonstrating "same notice, different reaction" purely through
    // each leaf's own state (isIndoors()), not type inspection. The
    // cascade itself runs through three runtime levels for the Keynote
    // Zone branch: root -> Keynote Zone -> Bridging the Divide/Coffee
    // Station.
    control.issueNotice(Notice(NoticeType::WEATHER_ALERT, "Sudden thunderstorm rolling in"));

    // SD3 - Conditional event response and Composite behaviour.
    // CAPACITY_ALERT for a LectureHall: Beyond the Algorithm is given a
    // current-attendance/threshold pair and is pushed over threshold;
    // Quantum Leaps is kept under threshold, so the same cascaded
    // CAPACITY_ALERT produces an "alt"-worthy branch (full vs not full)
    // between two instances of the very same concrete class purely from
    // object state - not from any type check. root's own aggregate
    // getCapacity() (a meaningful Composite operation) is queried
    // immediately beforehand so the two concepts - "how many seats
    // exist" vs "is a specific hall full right now" - are both visible
    // in the same interaction.
    std::cout << "\n--- SD3 scenario: capacity check ---\n";
    std::cout << "root->getCapacity() (Composite operation, recursive sum): "
              << root->getCapacity() << "\n";
    beyondTheAlgorithm->setCapacityThreshold(280);
    beyondTheAlgorithm->setCurrentAttendance(295); // over threshold -> alert
    quantumLeaps->setCapacityThreshold(280);
    quantumLeaps->setCurrentAttendance(150);       // under threshold -> no alert
    root->setCapacityThreshold(500);
    root->setCurrentAttendance(520);               // root itself also over threshold
    control.issueNotice(Notice(NoticeType::CAPACITY_ALERT, "Routine headcount check"));

    // TARGETED SCHEDULE_CHANGE: sent directly to one specific LectureHall
    // (SCHEDULE_CHANGE only ever affects LectureHall/WorkshopRoom/
    // RegistrationDesk/ExhibitionBooth, never a whole EventGroup), not
    // cascaded from the root - a delay to one talk does not move every
    // other talk's schedule.
    control.issueDirectNotice(beyondTheAlgorithm,
        Notice(NoticeType::SCHEDULE_CHANGE, "Speaker's flight delayed",
               -1, "10:30", "12:30"));

    // PAUSE/RESUME cascading from the root: only LectureHall,
    // WorkshopRoom, RegistrationDesk and ExhibitionBooth react.
    // MedicalStation, CateringStation, ShuttleStop and every EventGroup
    // are unaffected by these two notice types entirely.
    control.issueNotice(Notice(NoticeType::PAUSE, "Conference-wide safety pause"));
    control.issueNotice(Notice(NoticeType::RESUME, "Safety pause lifted"));

    // Also demonstrate a *targeted* PAUSE/RESUME aimed at just one
    // specific WorkshopRoom, leaving every other session untouched.
    control.issueDirectNotice(roboticsLab, Notice(NoticeType::PAUSE, "Localised issue: robot arm malfunction"));
    control.issueDirectNotice(roboticsLab, Notice(NoticeType::RESUME, "Robot arm repaired"));

    // TARGETED ANNOUNCEMENT: only EventGroups react to ANNOUNCEMENT.
    // Demonstrated both ways: cascaded conference-wide from the root,
    // and targeted at one specific EventGroup so it is only heard in
    // that area.
    control.issueNotice(Notice(NoticeType::ANNOUNCEMENT, "Lunch is now being served near Radisson Blu Catering"));
    control.issueDirectNotice(sponsorZone, Notice(NoticeType::ANNOUNCEMENT, "Prize draw at Booth QuantumEdge in 15 minutes"));

    // SD4 - Signature event scenario.
    // A system-wide POWER_OUTAGE affecting only MedicalStation and
    // CateringStation (their fallback reactions are their own form of
    // "pausing" - backup battery / cold menu). While recovery is
    // simulated ("the generator kicks in"), we also perform a runtime
    // reorganisation - reassigning ExhibitionBooth CloudNova from
    // Sponsor Zone directly under the root - so this single scenario
    // touches at least two Composite levels (Sponsor Zone and root) as
    // well as >= 6 lifelines (EventControl, root, Sponsor Zone,
    // CloudNova, First Aid, Radisson Blu Catering) and an
    // attach/detach-driven ownership change, matching every SD4
    // requirement. Finally a conference-wide RESUME is issued: it
    // affects the four PAUSE/RESUME-eligible leaf types (any that are
    // still paused) but - per policy - does *not* reach
    // MedicalStation/CateringStation, so their power-outage state is
    // deliberately left as the lasting effect of this scenario, itself
    // another example of "same notice, different (in this case, no)
    // reaction" driven by object type/state rather than a controller
    // checking types.
    std::cout << "\n--- SD4 scenario: power outage + runtime reorganisation ---\n";
    control.issueNotice(Notice(NoticeType::POWER_OUTAGE, "Loadshedding stage 4 -- switching to generator"));
    control.issueNotice(Notice(NoticeType::PAUSE, "Waiting for generator to kick-in..."));
    std::cout << "(...simulating a 5 minute wait while the generator starts...)\n";
    control.issueNotice(Notice(NoticeType::RESUME, "Generator online, full power restored"));

    std::cout << "Before: Sponsor Zone has " << sponsorZone->childCount()
              << " children, TechConnect Conference has " << root->childCount() << " children.\n";
    // removeChild() releases ownership from sponsorZone AND detaches
    // cloudNova from sponsorZone's observer list in one call; add() on
    // the new parent then re-establishes both ownership and observer
    // registration - the two relationships move together atomically
    // from the caller's point of view (Task 4.2). If the pointer
    // returned here were instead discarded without being add()ed
    // elsewhere, the caller would be responsible for `delete`ing it to
    // avoid a leak - removeChild() hands over real ownership, not just
    // a view.
    EventComponent* relocatedBooth = sponsorZone->removeChild(cloudNova);
    if (relocatedBooth) {
        root->add(relocatedBooth);
        std::cout << "Moved 'CloudNova' from Sponsor Zone directly under TechConnect Conference.\n";
    }
    std::cout << "After:  Sponsor Zone has " << sponsorZone->childCount()
              << " children, TechConnect Conference has " << root->childCount() << " children.\n";

    std::cout << "(...simulating a wait while the generator starts...)\n";
    control.issueNotice(Notice(NoticeType::RESUME, "Generator online -- pausable sessions may resume"));

    // REGISTRATION CHANGE at runtime: attach the medical station to
    // EventControl directly as well as its normal place in the tree,
    // demonstrating attach/detach independent of Composite ownership,
    // and that an observer can register with more than one subject.
    std::cout << "\n--- Registration change: First Aid now also observes EventControl directly ---\n";
    control.attach(firstAid);
    control.issueNotice(Notice(NoticeType::EVACUATE, "Evacuation drill: south exit"));
    std::cout << "First Aid received this notice twice: once cascaded via the root, "
                 "once directly from EventControl -- demonstrating attach() allows an "
                 "observer to register with more than one subject.\n";
    control.detach(firstAid);

    // CANCEL affects only LectureHall/WorkshopRoom/RegistrationDesk/
    // ExhibitionBooth, cascaded from the root, then clean shutdown.
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