#pragma once

class BaseComponent
{
public:
	bool IsActive() { return m_IsActve; }
private:

	bool m_IsActve;
}