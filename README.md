# EventFlow - TechConnect Conference

A reusable live-event coordination engine, implemented in C++11, built
around the GoF **Composite** and **Observer** patterns.


## 1. The event: TechConnect

TechConnect is a one-day technology conference held in a single
convention centre:

```
TechConnect Conference (09:00-17:00)                             [level 1]
  -> RegistrationDesk "Main Desk" (09:00-17:00)                  [level 2]
  -> Keynote Zone (10:00-16:00)                                  [level 2]
       -> LectureHall "Beyond the Algorithm" (10:00-12:00)       [level 3]
       -> LectureHall "Quantum Leaps" (13:00-15:00)
       -> LectureHall "Bridging the Divide" (16:00-17:00)  [outdoor]
       -> CateringStation "Coffee Station" (10:00-16:00)   [outdoor]
  -> Workshop Zone (13:00-17:00)                                 [level 2]
       -> WorkshopRoom "Robotics Lab" (13:00-15:00)              [level 3]
       -> WorkshopRoom "VR Lab" (15:00-17:00)
  -> Sponsor Zone (09:00-17:00)                                  [level 2]
       -> ExhibitionBooth "CloudNova" (09:00-17:00)              [level 3]
       -> ExhibitionBooth "QuantumEdge" (09:00-17:00)
  -> MedicalStation "First Aid" (09:00-17:00)                    [level 2]
  -> CateringStation "Radisson Blu Catering" (09:00-17:00)       [level 2]
  -> ShuttleStop "Gate 3 Shuttle Stop" (09:00-17:00) [always outdoor] [level 2]
```

That gives **7 concrete leaf types** (`LectureHall`, `WorkshopRoom`,
`RegistrationDesk`, `ExhibitionBooth`, `CateringStation`, `ShuttleStop`,
`MedicalStation`) and **3 levels of Composite nesting below the root**,
satisfying the non-negotiable structural minimums. Composites and
leaves are deliberately mixed at the same level (`RegistrationDesk`,
`MedicalStation`, `Radisson Blu Catering` and `Gate 3 Shuttle Stop` sit
directly under the root alongside the three zones), showing the client
can add either kind of `EventComponent` to a `EventGroup` without
distinction.

Any `EventComponent` can be outdoors -- indoor/outdoor is per-instance
state (`indoors_`), not a property of the class -- except `ShuttleStop`,
which is always outdoors by nature (a bus stop cannot be indoors) and
so has no `indoors` constructor parameter at all. In this tree only
`Gate 3 Shuttle Stop`, `Bridging the Divide` (a `LectureHall`) and
`Coffee Station` (a `CateringStation`) are outdoors; everything else is
indoors. Having an outdoor and an indoor instance of the *same* leaf
class (`LectureHall`, `CateringStation`) deliberately shows that "same
notice, different reaction" comes from each object's own state, not
just from which concrete class it is.

## 2. GoF roles

| Role (pattern)             | Class(es)                                   |
|-----------------------------|---------------------------------------------|
| Component (Composite)       | `EventComponent`                             |
| Leaf (Composite)             | `EventUnit` and its 7 concrete subclasses    |
| Composite (Composite)        | `EventGroup`                                 |
| Subject (Observer)           | `Subject` (abstract), implemented by `EventControl`, `EventGroup` |
| Observer (Observer)          | `Observer` (base), `EventUnit`, `EventGroup` |

`EventGroup` is deliberately **both** Observer and Subject: it hears
notices from its parent (Observer role) and re-broadcasts them to its
own children (Subject role). This is not a misuse of the pattern --
Composite answers "what does this group contain?" while Observer
answers "who does this group need to tell?", and those are genuinely
different questions with genuinely different collaborators (parent vs
children) even though the same object participates in both.

`EventControl` is **not** part of the Composite tree. It represents the
control-room staff at the client/root boundary and is a concrete Subject.

`Subject` is abstract, but not because `attach()`/`detach()`/`notify()`
are pure virtual -- they are ordinary virtual functions with a single,
fully working, reusable implementation in `Subject.cpp` that every
concrete Subject inherits unchanged. What actually keeps `Subject`
abstract is `describeSubject()`, a pure virtual operation every concrete
Subject implements differently (`EventControl` returns something like
`"EventControl[TechConnect 2026]"`, `EventGroup` returns
`"EventGroup[Keynote Zone]"`, etc.), used in EventFlow's status/log
output. This mirrors the class notes' `PitStop` example, where `PitStop`
already has complete implementations of `attach()`/`detach()`/`notify()`
yet stays abstract purely because it also declares its own
domain-specific operations (`tyre()`, `fuel()`, `setCar()`, `race()`) as
pure virtual for `FerrariStop` to implement.

### Field ownership across the Component hierarchy

`EventComponent` (the base of *both* `EventGroup` and `EventUnit`)
declares `name_`, `isOpen_`, `startTime_`/`endTime_` and
`currentAttendance_`/`capacityThreshold_` -- every kind of
`EventComponent`, Leaf or Composite, has a schedule and a capacity
threshold, because `OPEN`, `CLOSE`, `CAPACITY_ALERT` and `EVACUATE` are
required to affect all of them uniformly. Only `EventUnit` additionally
declares `indoors_` and `isPaused_`, since "is this physically indoors"
and "is this individual unit paused" are only ever meaningful for a
single operational unit, never for a whole `EventGroup` (a zone doesn't
have a location relative to weather, and it doesn't itself get
"paused" -- only the leaves inside it do, individually). Two protected
helpers on `EventComponent` -- `reportCapacityAlert()` and
`updateSchedule()` -- hold the one shared implementation for the
`CAPACITY_ALERT`/`SCHEDULE_CHANGE` reactions so that the many classes
that need them don't each re-implement the same comparison/print logic.

## 3. Cascaded vs. targeted notices

Every `Subject` can reach its observers two ways:

* **Cascaded**, via `EventControl::issueNotice()` -> `Subject::notify()`,
  which broadcasts to every currently attached observer and, for any
  observer that is itself a `Subject` (an `EventGroup`), continues
  cascading down through the whole subtree. Use this for genuinely
  conference-wide notices: `OPEN`, `WEATHER_ALERT`, `POWER_OUTAGE`,
  `CANCEL`, a global `PAUSE`/`RESUME`/`ANNOUNCEMENT`, etc.
* **Targeted**, via `EventControl::issueDirectNotice(target, notice)`,
  which delivers straight to one specific `EventGroup`/`EventUnit`'s
  `update()`, bypassing `EventControl`'s own registration list. If the
  target is itself an `EventGroup`, its own `notify()` still cascades
  the notice further down to *its* children as usual -- only the
  initial hop from `EventControl` is targeted rather than broadcast.
  Use this when a notice is inherently about one specific part of the
  venue: a `SCHEDULE_CHANGE` for one zone, an `ANNOUNCEMENT` local to
  just one area, or a `PAUSE` local to one room's AV fault.
  `OPEN`/`CLOSE`/`CANCEL`/`EVACUATE` can be sent either way -- cascaded
  for "the whole conference" or targeted for "just this zone" -- since
  both are meaningful (see `main.cpp`'s targeted `CLOSE` of Workshop
  Zone alongside the cascaded `OPEN`/`CANCEL` of the whole conference).

## 3. Cascaded vs. targeted notices

Every `Subject` can reach its observers two ways:

* **Cascaded**, via `EventControl::issueNotice()` -> `Subject::notify()`,
  which broadcasts to every currently attached observer and, for any
  observer that is itself a `Subject` (an `EventGroup`), continues
  cascading down through the whole subtree. Use this for genuinely
  conference-wide notices: `OPEN`, `WEATHER_ALERT`, `POWER_OUTAGE`,
  a global `PAUSE`/`RESUME`/`ANNOUNCEMENT`/`CANCEL`, etc.
* **Targeted**, via `EventControl::issueDirectNotice(target, notice)`,
  which delivers straight to one specific `EventGroup`/`EventUnit`'s
  `update()`, bypassing `EventControl`'s own registration list. If the
  target is itself an `EventGroup`, its own `notify()` still cascades
  the notice further down to *its* children as usual -- only the
  initial hop from `EventControl` is targeted rather than broadcast.
  Use this when a notice is inherently about one specific part of the
  venue: a `SCHEDULE_CHANGE` for one specific `LectureHall`, an
  `ANNOUNCEMENT` local to just one `EventGroup`, or a `PAUSE` local to
  one `WorkshopRoom`'s equipment fault.
  `OPEN`/`CLOSE`/`CANCEL`/`EVACUATE` can be sent either way -- cascaded
  for "the whole conference" or targeted for "just this leaf/zone" --
  since both are meaningful.

### Per-notice reaction policy

*"Affect" here always means "react to" -- a component that a notice
does not affect has no `case` for it at all in its `reactToNotice()`
and produces no output, rather than logging that it was unaffected.*

| Notice | Delivery | Who reacts |
|---|---|---|
| `OPEN` / `CLOSE` / `CAPACITY_ALERT` / `EVACUATE` | cascaded or targeted | **every** `EventComponent` -- all 7 leaf types and every `EventGroup` (including the root) |
| `PAUSE` / `RESUME` / `SCHEDULE_CHANGE` / `CANCEL` | cascaded or targeted | only `LectureHall`, `WorkshopRoom`, `RegistrationDesk`, `ExhibitionBooth` -- never `MedicalStation`, `CateringStation`, `ShuttleStop`, and never an `EventGroup` |
| `WEATHER_ALERT` | cascaded from root | only whichever leaves are *actually* outdoors right now (`isIndoors() == false`), regardless of leaf type -- checked via each leaf's own `indoors_` state, never its class |
| `POWER_OUTAGE` | cascaded from root | only `CateringStation` (cold-menu fallback) and `MedicalStation` (backup battery) |
| `ANNOUNCEMENT` | cascaded from root, **or targeted** at one `EventGroup` | only `EventGroup`s -- purely informational (logged, no state change), still cascades onward; no leaf type reacts to it at all |

One consequence worth calling out: because `RESUME` is in the
leaf-scoped group and `POWER_OUTAGE` is not, a `CateringStation`/
`MedicalStation` that switches to its fallback mode during a
`POWER_OUTAGE` is **not** reset by a later conference-wide `RESUME` --
that `RESUME` only reaches the four other leaf types. This is itself
another instance of "same notice, different (in this case, no)
reaction" driven purely by which classes have a `case` for it, not by
any controller checking types (see `main.cpp`'s SD4 scenario).

### Capacity thresholds

Every `EventComponent` -- leaf or group -- carries its own
`currentAttendance_`/`capacityThreshold_` pair (declared once in
`EventComponent`, see "Field ownership" above), set via
`setCurrentAttendance()`/`setCapacityThreshold()` (a simulated headcount
feed -- not derived from any `Notice`). When a `CAPACITY_ALERT` arrives,
the shared `EventComponent::reportCapacityAlert()` helper compares the
two and logs either "VENUE FULL" or a "not yet full" status; it does
nothing if `capacityThreshold_` was never set (still -1). `getCapacity()`
remains a *different* concept -- the recursive sum of every leaf's
fixed physical capacity -- so "how many seats exist" and "is it full
right now" are tracked and reasoned about independently.

## 4. Original features (Task 4.4)

TechConnect's three official original features are **`POWER_OUTAGE`**,
**`CANCEL`** and **`ANNOUNCEMENT`**. (`WEATHER_ALERT` also exists as an
extra notice type beyond the seven required by the specification and
follows the same "add one small `case` per interested class" principle
below, but is not counted among the three official answers to this
task.) Each of the three is deliberately scoped to a small,
already-existing subset of classes, so adding it only ever means adding
one `case` to the relevant leaves'/`EventGroup`'s existing
`reactToNotice()` switch -- never a new base class, a new cross-cutting
interface, or new responsibilities on `EventControl`. `EventControl`
still only ever knows how to broadcast or target-deliver a `Notice`; it
never needs to know which concrete leaf types care about which notice
type, which is what keeps it (and every other class) from growing into
a god object as the notice list grows:

* **`POWER_OUTAGE`** -- affects only `CateringStation` and
  `MedicalStation`. Every other leaf type's `reactToNotice()` simply
  never gained a `case` for it, so they fall through to their existing
  `default:` and stay completely unaffected; `EventGroup` needed no
  change at all, since it simply cascades every notice type it doesn't
  specifically care about.
* **`CANCEL`** -- affects `LectureHall`, `WorkshopRoom`,
  `RegistrationDesk` and `ExhibitionBooth` only, reusing the same
  `isOpen_` state and `default:` dispatch those four already had for
  `CLOSE`/`EVACUATE`; no new state was required anywhere, and
  `MedicalStation`/`CateringStation`/`ShuttleStop`/`EventGroup` simply
  never gained a `case` for it.
* **`ANNOUNCEMENT`** -- the opposite scoping to the other two: instead
  of touching several leaf types, it is handled *only* by `EventGroup`
  (one `case` in one class), since a PA-style announcement is inherently
  a property of an area, not of any individual unit inside it. No leaf
  type gained a `case`, a field, or any awareness that `ANNOUNCEMENT`
  exists at all -- it simply flows through their `update()`/`notify()`
  plumbing like any other notice they don't specifically handle. This
  demonstrates the other end of the same principle as the two
  leaf-scoped features above: adding a notice never requires touching
  every class, only the (possibly very small) set of classes that
  actually care.

## 5. Ownership policy

This project does **not** use `<memory>` or `<algorithm>` (by
requirement), so ownership is entirely explicit rather than delegated to
smart pointers, and list operations use hand-written loops instead of
`std::find`/`std::remove`.

* **Composite ownership**: `EventGroup` owns its children through a
  `std::vector<EventComponent*>`. `add()` and `removeChild()` together
  maintain the invariant that **every `EventComponent` is owned by at
  most one `EventGroup` at any moment**: `add()` takes ownership of a
  freshly-created child or of a pointer just released by
  `removeChild()`; `removeChild()` hands ownership to the caller and
  removes it from `children_` so the old owner will never `delete` it.
  `~EventGroup()` explicitly `delete`s every pointer still in
  `children_`, recursively tearing down the whole subtree exactly once.
  Destroying the root (`delete root;` in `main.cpp`) therefore destroys
  the entire owned subtree exactly once, with no manual bookkeeping
  needed beyond that single invariant.
* **Observer registration**: `Subject` stores **non-owning** raw
  `Observer*` pointers in a `std::vector`, managed with hand-written
  linear search/erase (`Subject::attach`/`detach`) rather than
  `std::find`/`std::remove`. Destroying a `Subject` never deletes an
  observer, and destroying an `Observer` never deletes a subject.
* **Coupling of the two**: `EventGroup::add()` transfers Composite
  ownership of a child *and* automatically registers it as an observer
  of the new parent. `EventGroup::removeChild()` reverses both in one
  call. This coupling is a deliberate design choice (anything physically
  inside a group should, by default, hear what affects that group) and
  is what makes runtime reorganisation (Task 4.2) a single, safe
  two-step operation: `parent->removeChild(x)` then `newParent->add(x)`.
  If the pointer returned by `removeChild()` is not subsequently
  `add()`ed anywhere, the caller becomes responsible for `delete`-ing it
  themselves to avoid a leak -- `removeChild()` hands over *real*
  ownership, not just a view.
* **Registrations independent of ownership are still possible**: nothing
  stops a client from calling `subject.attach(observer)` directly for an
  object that subject does *not* own (see the `First Aid`
  double-registration in `main.cpp`), demonstrating that Composite
  ownership and Observer registration are related but separate
  relationships in this design, and that an observer can register with
  more than one subject.

## 6. Push vs pull

EventFlow uses **push**. `Notice` carries everything an observer needs
(`type`, `message`, an optional informational `capacityThreshold`, and
`newStartTime`/`newEndTime` for `SCHEDULE_CHANGE`) and
`Subject::notify()`/`Observer::update()` pass the `Notice` directly.
This was chosen because TechConnect's notices are inherently
event-shaped (discrete occurrences, not continuously-polled state), so
there is no meaningful "current subject state" for an observer to pull
beyond the notice itself; push avoids observers needing a
back-reference to their subject just to ask "what changed?". Note that
`CAPACITY_ALERT` does *not* carry each object's attendance figures in
the `Notice` -- those are tracked locally on every `EventComponent`
via `setCurrentAttendance()`/`setCapacityThreshold()` (a simulated
headcount feed, set independently of any notice) precisely because each
object's own occupancy is naturally *its* state, not the control
centre's to package and broadcast.

## 7. Design-question policy summary

* Duplicate `attach()` of the same pointer: ignored (observer registered
  once).
* `detach()` of an unregistered observer: safe no-op.
* Observer changing registration mid-notification: `Subject::notify()`
  iterates a snapshot of the registration list, so an observer detaching
  itself during `update()` is safe and simply stops receiving *future*
  notices.
* An observer may register with more than one subject (see `main.cpp`).
* A Composite auto-registers each child it owns as an observer of
  itself; this coupling can still be bypassed by calling `attach()`/
  `detach()` directly when a different relationship is needed.

## 8. Building

```sh
make            # builds ./eventflow using -std=c++11 per the Makefile
./eventflow     # runs the integration demo
make mem
make clean
```
