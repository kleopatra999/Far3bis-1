﻿#ifndef MANAGER_HPP_C3173B86_845B_4D8D_921F_803EA43A3C8A
#define MANAGER_HPP_C3173B86_845B_4D8D_921F_803EA43A3C8A
#pragma once

/*
manager.hpp

Переключение между несколькими file panels, viewers, editors
*/
/*
Copyright © 1996 Eugene Roshal
Copyright © 2000 Far Group
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:
1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.
3. The name of the authors may not be used to endorse or promote products
   derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

class Viewer;

#include "windowsfwd.hpp"

class Manager: noncopyable
{
public:
	class Key
	{
	private:
		INPUT_RECORD m_Event;
		unsigned int m_FarKey;
		bool m_EventFilled;
		void Fill(unsigned int Key);
	public:
		Key(): m_Event(), m_FarKey(0), m_EventFilled(false) {}
		explicit Key(int Key);
		Key(unsigned int Key, const INPUT_RECORD& Event): m_Event(Event), m_FarKey(Key), m_EventFilled(true) {}
		const INPUT_RECORD& Event(void)const {return m_Event;}
		bool IsEvent(void)const {return m_EventFilled;}
		bool IsReal(void)const;
		Key& operator=(unsigned int Key);
		Key& operator&=(unsigned int Key);
		unsigned int operator()(void) const {return m_FarKey;}
	};

	class MessageAbstract;

	Manager();
	~Manager();

	enum DirectionType
	{
		PreviousWindow,
		NextWindow
	};

	// Эти функции можно безопасно вызывать практически из любого места кода
	// они как бы накапливают информацию о том, что нужно будет сделать с окнами при следующем вызове Commit()
	void InsertWindow(window_ptr_ref NewWindow);
	void DeleteWindow(window_ptr_ref Deleted = nullptr);
	void ActivateWindow(window_ptr_ref Activated);
	void RefreshWindow(window_ptr_ref Refreshed = nullptr);
	void ReplaceWindow(window_ptr_ref Old, window_ptr_ref New);
	void CallbackWindow(const std::function<void(void)>& Callback);
	//! Функции для запуска модальных окон.
	void ExecuteWindow(window_ptr_ref Executed);
	//! Входит в новый цикл обработки событий
	void ExecuteModal(window_ptr_ref Executed);
	//! Запускает немодальное окно в модальном режиме
	void ExecuteNonModal(window_ptr_ref NonModal);
	void RefreshAll(void);
	void CloseAll();
	/* $ 29.12.2000 IS
	Аналог CloseAll, но разрешает продолжение полноценной работы в фаре,
	если пользователь продолжил редактировать файл.
	Возвращает TRUE, если все закрыли и можно выходить из фара.
	*/
	BOOL ExitAll();
	size_t GetWindowCount()const { return m_windows.size(); }
	int  GetWindowCountByType(int Type);
	/*$ 26.06.2001 SKV
	Для вызова через ACTL_COMMIT
	*/
	void PluginCommit();
	int CountWindowsWithName(const string& Name, BOOL IgnoreCase = TRUE);
	bool IsPanelsActive(bool and_not_qview = false) const; // используется как признак Global->WaitInMainLoop
	window_ptr FindWindowByFile(int ModalType, const string& FileName, const wchar_t *Dir = nullptr);
	void EnterMainLoop();
	void ProcessMainLoop();
	void ExitMainLoop(int Ask);
	int ProcessKey(Key key);
	int ProcessMouse(const MOUSE_EVENT_RECORD *me);
	void PluginsMenu() const; // вызываем меню по F11
	void SwitchToPanels();
	window_ptr GetCurrentWindow() { return m_currentWindow; }
	window_ptr GetWindow(size_t Index) const;
	int IndexOf(window_ptr_ref Window);
	int IndexOfStack(window_ptr_ref Window);
	window_ptr GetBottomWindow() { return m_windows.back(); }
	bool ManagerIsDown() const { return EndLoop; }
	bool ManagerStarted() const { return StartManager; }
	void InitKeyBar();
	bool InModal(void) const { return !m_modalWindows.empty(); }
	void ResizeAllWindows();
	size_t GetModalWindowCount() const { return m_modalWindows.size(); }
	window_ptr GetModalWindow(size_t index) const { return m_modalWindows[index]; }

	void AddGlobalKeyHandler(const std::function<int(const Key&)>& Handler);

	static long GetCurrentWindowType() { return CurrentWindowType; }
	static bool ShowBackground();

	void UpdateMacroArea(void);

	typedef std::set<window_ptr, std::function<bool(window_ptr_ref, window_ptr_ref)>> sorted_windows;
	sorted_windows GetSortedWindows(void) const;

	Viewer* GetCurrentViewer(void) const;
	FileEditor* GetCurrentEditor(void) const;
	window_ptr GetViewerContainerById(int ID) const;
	window_ptr GetEditorContainerById(int ID) const;
	// BUGBUG, do we need this?
	void ImmediateHide();
	bool HaveAnyMessage() const;

private:
#if defined(SYSLOG)
	friend void ManagerClass_Dump(const wchar_t *Title, FILE *fp);
#endif

	window_ptr WindowMenu(); //    вместо void SelectWindow(); // show window menu (F12)
	bool HaveAnyWindow() const;
	bool OnlyDesktop() const;
	void Commit(void);         // завершает транзакцию по изменениям в контейнерах окон
	// Она в цикле вызывает себя, пока хотябы один из указателей отличен от nullptr
	// Функции, "подмастерья начальника" - Commit'a
	// Иногда вызываются не только из него и из других мест
	void InsertCommit(window_ptr_ref Param);
	void DeleteCommit(window_ptr_ref Param);
	void ActivateCommit(window_ptr_ref Param);
	void RefreshCommit(window_ptr_ref Param);
	void DeactivateCommit(window_ptr_ref Param);
	void ExecuteCommit(window_ptr_ref Param);
	void ReplaceCommit(window_ptr_ref Old, window_ptr_ref New);
	int GetModalExitCode() const;

	typedef void(Manager::*window_callback)(window_ptr_ref);

	void PushWindow(window_ptr_ref Param, window_callback Callback);
	void CheckAndPushWindow(window_ptr_ref Param, window_callback Callback);
	void RedeleteWindow(window_ptr_ref Deleted);
	bool AddWindow(window_ptr_ref Param);
	void SwitchWindow(DirectionType Direction);

	window_ptr m_currentWindow;     // текущее окно. Оно может находиться как в немодальном, так и в модальном контейнере, его можно получить с помощью WindowManager->GetCurrentWindow();
	typedef std::vector<window_ptr> windows;
	void* GetCurrent(std::function<void*(windows::const_reverse_iterator)> Check) const;
	windows m_modalWindows;
	windows m_windows;
	// текущее немодальное окно можно получить с помощью WindowManager->GetBottomWindow();
	/* $ 15.05.2002 SKV
		Так как есть полумодалы, что б не было путаницы,
		заведём счётчик модальных editor/viewer'ов.
		Дёргать его  надо ручками перед вызовом ExecuteModal.
		А автоматом нельзя, так как ExecuteModal вызывается
		1) не только для настоящих модалов (как это ни пародоксально),
		2) не только для editor/viewer'ов.
	*/
	bool EndLoop;            // Признак выхода из цикла
	int ModalExitCode;
	bool StartManager;
	static long CurrentWindowType;
	std::list<std::unique_ptr<MessageAbstract>> m_Queue;
	std::vector<std::function<int(const Key&)>> m_GlobalKeyHandlers;
	std::unordered_map<window_ptr, bool*> m_Executed;
	std::unordered_set<window_ptr> m_Added;
};

#endif // MANAGER_HPP_C3173B86_845B_4D8D_921F_803EA43A3C8A
