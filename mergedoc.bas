Option Explicit

SUB MergeDocs
    DIM FilePicker AS Object
    DIM SelectedFiles AS Variant
    DIM Document AS Object
    DIM Dispatcher AS Object
    DIM DlgResult AS Integer
    DIM InsertDocProperties(1) AS NEW com.sun.star.beans.PropertyValue
    DIM i AS Integer

    FilePicker = CreateUnoService("com.sun.star.ui.dialogs.FilePicker")

    FilePicker.SetMultiSelectionMode(true)
    FilePicker.SetTitle("Select files to merge")
    FilePicker.AppendFilter("All Files (*.*)", "*.*")
    FilePicker.AppendFilter("Microsoft Word documents (*.doc)", "*.doc")
    FilePicker.SetCurrentFilter("Microsoft Word documents (*.doc)")
    FilePicker.SetDisplayDirectory(ConvertToUrl("/tmp/docs"))
    DlgResult = FilePicker.Execute()

    IF DlgResult <> IDOK THEN
        EXIT SUB
    END IF

    SelectedFiles = FilePicker.GetSelectedFiles()

    FOR i = LBound(SelectedFiles()) TO UBound(SelectedFiles())
        InsertDocProperties(0).Name = "Name"
        InsertDocProperties(0).Value = SelectedFiles(i)
        InsertDocProperties(1).Name = "Filter"
        InsertDocProperties(1).Value = "MS Word 97"

        Document = ThisComponent.CurrentController.Frame()
        Dispatcher = CreateUnoService("com.sun.star.frame.DispatchHelper")
        Dispatcher.ExecuteDispatch(Document, ".uno:InsertDoc", "", 0, InsertDocProperties())
        Dispatcher.ExecuteDispatch(Document, ".uno:InsertPara", "", 0, Array())
    NEXT
END SUB
