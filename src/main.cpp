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
#include "vtkScalarBarActor.h"

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
#include "vtkImageGaussianSmooth.h"


VTK_MODULE_INIT(vtkRenderingOpenGL2)
VTK_MODULE_INIT(vtkInteractionStyle)

int main(int argc, char** argv)
{
	// Setup the reader
	vtkSmartPointer<vtkXMLUnstructuredGridReader> reader =
		vtkSmartPointer<vtkXMLUnstructuredGridReader>::New();
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


	/*vtkSmartPointer<vtkImageGaussianSmooth> gauss =
		vtkSmartPointer<vtkImageGaussianSmooth>::New();
	gauss->SetInputConnection(reader->GetOutputPort());
	gauss->SetRadiusFactor(0.5);*/

	// Mapper
	vtkNew<vtkDataSetMapper> Mapper;
	Mapper->SetInputConnection(reader->GetOutputPort());
	Mapper->SetScalarRange(reader->GetOutputAsDataSet()->GetScalarRange());
	Mapper->InterpolateScalarsBeforeMappingOn();

	//Scalar Bar
	vtkSmartPointer<vtkScalarBarActor> scalarBar =
		vtkSmartPointer<vtkScalarBarActor>::New();
    scalarBar->SetLookupTable(Mapper->GetLookupTable());
	scalarBar->SetTitle("Intensity");
	scalarBar->SetNumberOfLabels(10);

	// Actor
	vtkNew<vtkActor> Actor;
	Actor->SetMapper(Mapper.Get());
	Actor->GetProperty()->SetColor(0.2, 0, 0.8);
	renderer->AddActor(Actor.Get());
	renderer->AddActor2D(scalarBar);

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