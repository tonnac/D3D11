#include "FinateStateMachine.h"

FinateState::FinateState()
{
	m_mapOutput.clear();
}

void FinateState::AddTransition(const E_EVENT& eventtype, const std::tstring& statetype)
{
	m_mapOutput[eventtype] = statetype;
}
std::tstring FinateState::GetState(const E_EVENT& eventtype)
{
	return m_mapOutput[eventtype];
}
FinateStateMachine::FinateStateMachine()
{
	m_mapState.clear();
}
FinateStateMachine::~FinateStateMachine()
{
}
void FinateStateMachine::AddStateTransition(const std::tstring& inputState,
						const E_EVENT& eventtype, const std::tstring& outputState)
{
	FinateState * nstate = nullptr;
	EnemyIterator iter = m_mapState.find(inputState);
	if (iter == m_mapState.end())
	{
		nstate = new FinateState;
		m_mapState[inputState] = nstate;
	}
	else
	{
		nstate = iter->second;
	}
	nstate->AddTransition(eventtype, outputState);
}
std::tstring FinateStateMachine::StateTransition(const std::tstring& currentstate, const E_EVENT& eventtype)
{
	return m_mapState[currentstate]->GetState(eventtype);
}