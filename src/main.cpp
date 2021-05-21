#include "vtkActor.h"
#include "vtkContourFilter.h"
#include "vtkPolyDataMapper.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkSmartPointer.h"
#include "vtkXMLImageDataReader.h"
#include "vtkXMLUnstructuredGridReader.h"
#include "vtkDataSetMapper.h"
#include "vtkAutoInit.h"
#include <vtkNew.h>
#include "vtkInteractorStyleTrackballCamera.h"
#include "vtkPlane.h"
#include "vtkClipDataSet.h"
#include "vtkPointSource.h"

#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkLookupTable.h>
#include <vtkNew.h>
#include <vtkPlane.h>
#include <vtkPointData.h>
#include <vtkPointSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>


VTK_MODULE_INIT(vtkRenderingOpenGL2)
VTK_MODULE_INIT(vtkInteractionStyle)

int main(int argc, char** argv)
{
	// Setup the reader
	vtkSmartPointer<vtkXMLUnstructuredGridReader> reader =
		vtkSmartPointer<vtkXMLUnstructuredGridReader>::New();
	//reader->SetFileName("C:\\Users\\alsol\\Documents\\Git\\CG_course\\test_1\\data\\TriQuadraticHexahedron.vtu");
	reader->SetFileName("C:\\Users\\alsol\\Documents\\Git\\CG_course\\data\\notch_disp.vtu");
	reader->Update();

	// Setup the renderer
	vtkNew<vtkRenderer> renderer;
	renderer->SetBackground(0.9, 0.9, 0.9);

	// Setup the render window
	vtkNew<vtkRenderWindow> renWin;
	renWin->AddRenderer(renderer.Get());
	renWin->SetSize(500, 500);

	// Setup the render window interactor
	vtkNew<vtkRenderWindowInteractor> interact;
	vtkNew<vtkInteractorStyleTrackballCamera> style;
	interact->SetRenderWindow(renWin.Get());
	interact->SetInteractorStyle(style.Get());

	// Clip the left part from the input
	vtkNew<vtkPlane> planeLeft;
	planeLeft->SetOrigin(0.0, 0.0, 0.0);
	planeLeft->SetNormal(-1.0, 0.0, 0.0);

	vtkNew<vtkClipDataSet> clipperLeft;
	clipperLeft->SetInputConnection(reader->GetOutputPort());
	clipperLeft->SetClipFunction(planeLeft.Get());


	// Create Mapper and Actor for the left part
	vtkNew<vtkDataSetMapper> leftWireMapper;
	leftWireMapper->SetInputConnection(clipperLeft->GetOutputPort());
	leftWireMapper->SetScalarRange(reader->GetOutputAsDataSet()->GetScalarRange());
	leftWireMapper->InterpolateScalarsBeforeMappingOn();

	vtkNew<vtkActor> leftWireActor;
	leftWireActor->SetMapper(leftWireMapper.Get());
	leftWireActor->GetProperty()->SetColor(0.2, 0, 0.8);
	renderer->AddActor(leftWireActor.Get());


	// Create Mapper and Actor for the right part
	vtkNew<vtkPlane> planeRight;
	planeRight->SetOrigin(0.0, 0.0, 0.0);
	planeRight->SetNormal(1.0, 0.0, 0.0);

	vtkNew<vtkClipDataSet> clipperRight;
	clipperRight->SetInputConnection(reader->GetOutputPort());
	clipperRight->SetClipFunction(planeRight.Get());

	// Create the wireframe representation for the right part
	vtkNew<vtkDataSetMapper> rightWireMapper;
	rightWireMapper->SetInputConnection(clipperRight->GetOutputPort());
	rightWireMapper->SetScalarRange(reader->GetOutputAsDataSet()->GetScalarRange());
	rightWireMapper->InterpolateScalarsBeforeMappingOn();

	vtkNew<vtkActor> rightWireActor;
	rightWireActor->SetMapper(rightWireMapper.Get());
	rightWireActor->GetProperty()->SetColor(0.2, 0, 1);
	renderer->AddActor(rightWireActor.Get());


	// **********************************************************************************************

	//vtkNew<vtkDataSetMapper> pressureColorMapper;
	//pressureColorMapper->SetInputConnection(clipperLeft->GetOutputPort());
	//pressureColorMapper->SelectColorArray("Nodal Displacement");
	//spressureColorMapper->SetScalarRange(pressureRange);

	//vtkNew<vtkActor> pressureColorActor;
	//pressureColorActor->SetMapper(pressureColorMapper.Get());
	//pressureColorActor->GetProperty()->SetOpacity(0.5);
	//renderer->AddActor(pressureColorActor.Get());

	renWin->Render();
	interact->Initialize();
	interact->Start();
	
	return 0;
}