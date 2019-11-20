#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkContourFilter.h>
#include <vtkLookupTable.h>
#include <vtkNamedColors.h>
#include <vtkOutlineFilter.h>
#include <vtkPointData.h>
#include <vtkPointSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkStreamTracer.h>
#include <vtkStructuredPointsReader.h>
#include <vtkThresholdPoints.h>
#include <vtkConeSource.h>
#include <vtkTubeFilter.h>
#include <vtkMaskPoints.h>
#include <vtkGlyph3D.h>
#include <vtkArrowSource.h>
#include <vtkHedgeHog.h>

int main (int argc, char *argv[]){
  

  vtkSmartPointer<vtkNamedColors> colors = vtkSmartPointer<vtkNamedColors>::New();
  vtkSmartPointer<vtkRenderer> ren1 = vtkSmartPointer<vtkRenderer>::New();

  vtkSmartPointer<vtkRenderWindow> renWin = vtkSmartPointer<vtkRenderWindow>::New();
                                   renWin->AddRenderer(ren1);

  vtkSmartPointer<vtkRenderWindowInteractor> iren = vtkSmartPointer<vtkRenderWindowInteractor>::New();
                                             iren->SetRenderWindow(renWin);

// create pipeline
//


  vtkSmartPointer<vtkStructuredPointsReader> reader = vtkSmartPointer<vtkStructuredPointsReader>::New();
                                             reader->SetFileName("velocity.dat");
											 reader->Update();

vtkSmartPointer<vtkArrowSource>  arrow = vtkSmartPointer<vtkArrowSource>::New();
							     arrow->SetTipResolution(6);
								 arrow->SetTipRadius(0.125);
								 arrow->SetTipLength(0.4);
								 arrow->SetShaftResolution(6);
								 arrow->SetShaftRadius(0.075);


vtkSmartPointer<vtkConeSource> cone = vtkSmartPointer<vtkConeSource>::New();
							   cone->SetResolution(3);
							   cone->SetHeight(1);
							   cone->SetRadius(0.075);

vtkSmartPointer<vtkGlyph3D> cones = vtkSmartPointer<vtkGlyph3D>::New();
							   cones->SetInputConnection(reader->GetOutputPort()); //pass data as is
							   cones->SetSourceConnection(cone->GetOutputPort()); //arrow or cone
							   cones->SetScaleFactor(2.0);//make arrow or cone much larger
							   cones->SetScaleModeToScaleByScalar();
							   cones->SetVectorModeToUseVector();
							   cones->OrientOn();



  /*double range[2];
  range[0] = cones->GetOutput()->GetPointData()->GetScalars()->GetRange()[0];
  range[1] = cones->GetOutput()->GetPointData()->GetScalars()->GetRange()[1];
  
  printf("Range[0] is %f\n", range[0]);
  printf("Range[1] is %f\n", range[1]);*/


 vtkSmartPointer<vtkLookupTable> lut = vtkSmartPointer<vtkLookupTable>::New();
                                  lut->SetHueRange(.667, 0.0);
                                  lut->Build();

vtkSmartPointer<vtkPolyDataMapper> vecMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
								  vecMapper->SetInputConnection(cones->GetOutputPort());
								  vecMapper->SetScalarRange(0, 1);
								  vecMapper->SetLookupTable(lut);
		
								 

 vtkSmartPointer<vtkActor> streamerActor = vtkSmartPointer<vtkActor>::New();
                            streamerActor->SetMapper(vecMapper);


// outline
  vtkSmartPointer<vtkOutlineFilter> outline = vtkSmartPointer<vtkOutlineFilter>::New();
                                    outline->SetInputConnection(reader->GetOutputPort());

  vtkSmartPointer<vtkPolyDataMapper> outlineMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
                                     outlineMapper->SetInputConnection(outline->GetOutputPort());

  vtkSmartPointer<vtkActor> outlineActor = vtkSmartPointer<vtkActor>::New();
                            outlineActor->SetMapper(outlineMapper);
                            outlineActor->GetProperty()->SetColor(colors->GetColor3d("Black").GetData());

// Add the actors to the renderer, set the background and size
//
  ren1->AddActor(outlineActor);
  ren1->AddActor(streamerActor);
  ren1->SetBackground(colors->GetColor3d("Black").GetData());
  renWin->SetSize(640, 480);

  vtkSmartPointer<vtkCamera> aCamera = vtkSmartPointer<vtkCamera>::New();
  aCamera->SetViewUp(0, 1, 0);
  aCamera->SetPosition(0, 0, 1);
  aCamera->SetFocalPoint(0, 0, 0);
  aCamera->ComputeViewPlaneNormal();
  ren1->SetActiveCamera(aCamera);
  ren1->ResetCamera();
  ren1->ResetCameraClippingRange();
  /*
  vtkSmartPointer<vtkCamera> cam1 = vtkSmartPointer<vtkCamera>::New();
                             cam1->SetClippingRange(17.4043, 870.216);
                             cam1->SetFocalPoint(136.71, 104.025, 23);
                             cam1->SetPosition(204.747, 258.939, 63.7925);
                             cam1->SetViewUp(-0.102647, -0.210897, 0.972104);
                             cam1->Zoom(1.6);
                             ren1->SetActiveCamera(aCamera);*/

// render the image
//
  renWin->Render();
  iren->Start();

  return EXIT_SUCCESS;
}
