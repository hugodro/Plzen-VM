/**************************************************
* File: fsmSupport.cc.
* Desc: Implementation of the FSM support classes, PVState, PVTransition, .
* Module: AkraLog : JavaKit.
* Rev: 26 mai 1996 : REV 0 : Hugo DesRosiers : Creation.
**************************************************/

#include "classAttributes.h"
#include "classFile.h"
#include "eventQueue.h"
#include "fsmSupport.h"


#if defined(PLZEN_LEVEL_2)
/**************************************************
* Implementation: PVState.
**************************************************/

PVState::PVState(void)
{
  // Should throw an error, there is no membership for the method.
    flags= 0;
}


PVState::PVState(ClassFile *owner, MethodInfo *info) : JVMethod(owner, info)
{
    StateCodeAttribute *codeAttribute;
    TransitionsAttribute *tmpAttr;
    unsigned int i, j, k;
    bool hasDefault= false;

    defaultTransition= NULL;
    nbrLocalTransitions= nbrGlobalTransitions= 0;
    flags= 0;

    if ((codeAttribute= (StateCodeAttribute *)info->getCodeInfo()) != NULL) {
	for (i= 0; i < codeAttribute->attributeCount; i++) {
	    if (codeAttribute->attributes[i]->category() == ATTR_TRANSITIONS) {
		if (((TransitionsAttribute *)codeAttribute->attributes[i])->isModified(MOD_DEFAULT)) {
		    defaultTransition= new PVTransition((TransitionsAttribute *)codeAttribute->attributes[i]);
		    hasDefault= true;
		}
		else {
		    if (!((TransitionsAttribute *)codeAttribute->attributes[i])->isModified(MOD_FIELD)) {
			nbrLocalTransitions++;
		    }
		    else {
			nbrGlobalTransitions++;
		    }
		}
	    }
	}
    
	if (nbrLocalTransitions > 0) {
	    localTransitions= new PVLocalTransition[nbrLocalTransitions];
	}
	if (nbrGlobalTransitions > 0) {
	    transitions= new PVFieldTransition[nbrGlobalTransitions];
	}
    
	for (i= j= k= 0; i < codeAttribute->attributeCount; i++) {
	    if (codeAttribute->attributes[i]->category() == ATTR_TRANSITIONS) {
		if (((TransitionsAttribute *)codeAttribute->attributes[i])->isModified(MOD_DEFAULT)) {
		    continue;
		}
		else if (((TransitionsAttribute *)codeAttribute->attributes[i])->isModified(MOD_FIELD)) {
		    transitions[k++].linkWith((TransitionsAttribute *)codeAttribute->attributes[i]);
		}
		else {
		    localTransitions[j++].linkWith((TransitionsAttribute *)codeAttribute->attributes[i]);
		}
		if (((TransitionsAttribute *)codeAttribute->attributes[i])->isModified(MOD_BLOCKING)) {
		    flags|= blockingTrans;
		}
		if (((TransitionsAttribute *)codeAttribute->attributes[i])->isModified(MOD_IMMEDIATE)) {
		    flags|= immediateTrans;
		}
	    }
	}

    }

}


bool PVState::resolveTransitions(ClassManager *classManager, ClassFile *owner)
{
    unsigned int i;

    if (defaultTransition != NULL) {
	if (!defaultTransition->resolve(classManager, owner)) return false;
    }

    for (i= 0; i < nbrLocalTransitions; i++) {
	if (!localTransitions[i].resolve(classManager, owner)) return false;
    }

    for (i= 0; i < nbrGlobalTransitions; i++) {
	if (!transitions[i].resolve(classManager, owner)) return false;
    }

    return true;
}


PVTransition *PVState::getDefaultTransition(void)
{
    return defaultTransition;
}



PVTransition *PVState::getMatchFor(PVEvent *anEvent)
{
    unsigned int i;

    if (anEvent->isLocal()) {
	for (i= 0; i < nbrLocalTransitions; i++) {
	    if (localTransitions[i].isMatching(anEvent->getValue())) {
		return &localTransitions[i];
	    }
	}
    }
    else {
	for (i= 0; i < nbrGlobalTransitions; i++) {
	    if (transitions[i].isMatching(anEvent->getValue())) {
		return &transitions[i];
	    }
	}
    }
    return NULL;
}


bool PVState::hasBlockingTransitions(void)
{
    return ((flags & blockingTrans) != 0);
}


bool PVState::hasImmediateTransitions(void)
{
    return ((flags & immediateTrans) != 0);
}


/**************************************************
* Implementation: PVTransition.
**************************************************/

PVTransition::PVTransition(void)
{
    original= NULL;
    modifiers= 0;
    destination= NULL;
    paramLoader= 0;
}


PVTransition::PVTransition(TransitionsAttribute *source)
{
    original= source;
    modifiers= source->modifiers;
    destination= NULL;
    paramLoader= source->startPC;
}


void PVTransition::linkWith(TransitionsAttribute *source)
{
    original= source;
    modifiers= source->modifiers;
    paramLoader= source->startPC;
}


bool PVTransition::resolve(ClassManager *classManager, ClassFile *owner)
{
    if (!(modifiers & (MOD_PASSBY | MOD_CONSUME | MOD_EXIT))) {
	if ((destination= (MethodInfo *)((CteMethodRef *)owner->getConstant(original->destinationIndex))->resolve(classManager, owner)) == NULL) {
	    return false;
	}
    }

    return true;
}


MethodInfo *PVTransition::getDestination(void)
{
    return destination;
}


unsigned int PVTransition::getParamsPC(void)
{
    return paramLoader;
}


bool PVTransition::isMatching(JVInstance *aValue)
{
    return false;		// ATTN: Default action is not to know the trigger.
}


unsigned int PVTransition::getModifiers(void)
{
    return modifiers;
}


/**************************************************
* Implementation: PVLocalTransition.
**************************************************/

PVLocalTransition::PVLocalTransition(void)
{
    trigger= NULL;
}


bool PVLocalTransition::isMatching(JVInstance *aValue)
{
	// TODO-970528 [HD]: Implement the right matching.
    return (((void *)trigger) == ((void *)aValue));
//    return ((AkObject *)trigger == (AkObject *)aValue);
}


/**************************************************
* Implementation: PVFieldTransition.
**************************************************/

PVFieldTransition::PVFieldTransition(void)
{
    trigger= NULL;
}


bool PVFieldTransition::resolve(ClassManager *classManager, ClassFile *owner)
{
    PVTransition::resolve(classManager, owner);
    if (!(modifiers & (MOD_PASSBY | MOD_CONSUME | MOD_EXIT))) {
	if ((trigger= (FieldInfo *)((CteFieldRef *)owner->getConstant(original->eventIndex))->resolve(classManager, owner)) == NULL) {
	    return false;
	}
    }

    return true;
}


bool PVFieldTransition::isMatching(JVInstance *aValue)
{
	// TODO-970528 [HD]: Implement the right matching.
//    if ((void *)trigger == (void *)aValue) return true;
//    else return false;
    return ((void *)trigger == (void *)aValue);
}

#endif	    /* PLZEN_LEVEL_2 */

