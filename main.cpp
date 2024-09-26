#include <wx/wx.h>
#include <wx/datetime.h>
#include <wx/timer.h>
#include <vector>

class Task {
public:
    wxString description;
    wxDateTime dueDateTime;

    Task(const wxString& desc, const wxDateTime& dt) : description(desc), dueDateTime(dt) {}
};

class ToDoListFrame : public wxFrame {
public:
    ToDoListFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
        : wxFrame(NULL, wxID_ANY, title, pos, size), m_timer(this, wxID_ANY) {

        SetMinSize(wxSize(400, 300));

        // UI Elements
        wxPanel* panel = new wxPanel(this, wxID_ANY);
        m_listBox = new wxListBox(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, nullptr, wxLB_SINGLE);
        m_textCtrl = new wxTextCtrl(panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
        m_dateTimePicker = new wxDatePickerCtrl(panel, wxID_ANY, wxDefaultDateTime);
        m_timePicker = new wxTimePickerCtrl(panel, wxID_ANY);
        wxButton* addButton = new wxButton(panel, wxID_ANY, "Add Task");

        // Layout
        wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
        sizer->Add(m_textCtrl, 0, wxEXPAND | wxALL, 5);
        sizer->Add(m_dateTimePicker, 0, wxEXPAND | wxALL, 5);
        sizer->Add(m_timePicker, 0, wxEXPAND | wxALL, 5);
        sizer->Add(addButton, 0, wxALIGN_CENTER | wxALL, 5);
        sizer->Add(m_listBox, 1, wxEXPAND | wxALL, 5);
        panel->SetSizer(sizer);

        // Event bindings
        addButton->Bind(wxEVT_BUTTON, &ToDoListFrame::OnAddTask, this);
        m_textCtrl->Bind(wxEVT_TEXT_ENTER, &ToDoListFrame::OnAddTask, this);
        m_timer.Bind(wxEVT_TIMER, &ToDoListFrame::OnTimer, this);

        m_timer.Start(60000); // Check every minute
    }

private:
    std::vector<Task> m_tasks;
    wxListBox* m_listBox;
    wxTextCtrl* m_textCtrl;
    wxDatePickerCtrl* m_dateTimePicker;
    wxTimePickerCtrl* m_timePicker;
    wxTimer m_timer;

    // Add a new task
    void OnAddTask(wxCommandEvent& event) {
        wxString taskDescription = m_textCtrl->GetValue();
        wxDateTime date = m_dateTimePicker->GetValue();
        wxDateTime time = m_timePicker->GetValue();
        date.SetHour(time.GetHour());
        date.SetMinute(time.GetMinute());

        if (!taskDescription.IsEmpty() && date.IsValid() && date > wxDateTime::Now()) {
            Task newTask(taskDescription, date);
            m_tasks.push_back(newTask);
            m_listBox->Append(newTask.description);
            m_textCtrl->Clear();
        } else {
            wxMessageBox("Please enter a valid description and future date/time.", "Invalid Input", wxOK | wxICON_WARNING);
        }
    }

    // Timer event that checks for due tasks
    void OnTimer(wxTimerEvent& event) {
        wxDateTime now = wxDateTime::Now();
        for (auto& task : m_tasks) {
            wxTimeSpan timeLeft = task.dueDateTime - now;
            if (timeLeft.IsPositive() && timeLeft.GetSeconds() <= 3600) {
                wxString msg = wxString::Format("Task '%s' is due in less than an hour!", task.description);
                wxMessageBox(msg, "Reminder", wxICON_INFORMATION);
            }
        }
    }

    wxDECLARE_EVENT_TABLE();
};


wxBEGIN_EVENT_TABLE(ToDoListFrame, wxFrame)

wxEND_EVENT_TABLE()

class ToDoListApp : public wxApp {
public:
    virtual bool OnInit() {
        ToDoListFrame* frame = new ToDoListFrame("To-Do List", wxPoint(50, 50), wxSize(800, 600));
        frame->Show(true);
        return true;
    }
};

wxIMPLEMENT_APP(ToDoListApp);
