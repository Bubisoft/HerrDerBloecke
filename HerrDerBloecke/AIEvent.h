#pragma once
using namespace System;
using namespace SlimDX;

namespace HdB{
    ref class Unit;
    ref class Soldier;
    enum EventStatus{NEW,OPEN,CLOSED};

    ref class AIEvent
    {
    public:
        AIEvent(UInt64 attime, UInt16 priority): mAtTime(attime), mPriority(priority), mStatus(HdB::EventStatus::NEW) {}
    protected:
        UInt64 mAtTime;
        UInt16 mPriority; // higher value -> higher priority  
        EventStatus mStatus;
    public:
        property EventStatus Status{
            EventStatus get() {return mStatus;}
            void set(EventStatus status) { mStatus=status; }
        }

        property UInt64 AtTime{
            UInt64 get() { return mAtTime; }
        }
        property UInt64 Priority{
            UInt64 get() {return mPriority;}
        }
    };

    ref class AISoldierEvent: AIEvent
    {
    private:
        Soldier^ mSoldier;
        Unit^ mTarget;
        Unit^ mOldTarget;
        bool mIsDefense;
    public:
        AISoldierEvent(UInt64 attime, UInt16 priority,Soldier^ soldier,Unit^ target, Unit^ oldtarget, bool isdefense): AIEvent(attime,priority), mSoldier(soldier), mTarget(target), mOldTarget(oldtarget), mIsDefense(isdefense) { }
    public:
        property bool IsDefense{
            bool get() {return mIsDefense;}        
        }
        property Soldier^ Soldier{
            HdB::Soldier^ get() { return mSoldier; }
        }
        property Unit^ Target{
            HdB::Unit^ get() { return mTarget; }
        }
        property Unit^ OldTarget{
            HdB::Unit^ get() { return mOldTarget; }
        }

    };

    ref class AIUnitEvent : AIEvent
    {
    private:
        Unit^ mUnit;
        Unit^ mAlpha;
    public:
        AIUnitEvent(UInt64 attime, UInt16 priority, Unit^ u, Unit^ alpha): AIEvent(attime, priority), mUnit(u), mAlpha(alpha) { }
    public:
        property HdB::Unit^ Unit{
            HdB::Unit^ get() { return mUnit; }
        }
        property HdB::Unit^ Alpha{
            HdB::Unit^ get() { return mAlpha; }
        }
    };

}
