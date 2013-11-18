#pragma once

#include "DiagramLayer.h"

class CDiagramLayerSelector
{
public:
	CDiagramLayerSelector();
	~CDiagramLayerSelector(void);

	CDiagramLayer* GetDiagramLayer(CString strLayerName,CodeInfo* pCodeInfo);
	void SetHostWnd(CWnd* pHostWnd) { m_pHostWnd = pHostWnd; } 
	CString GetStandardFileName(CString strLayerName,CodeInfo* pCodeInfo);

private:
	CTypedPtrMap<CMapStringToPtr,CString,CDiagramLayer*>  m_layerMap;
	CWnd* m_pHostWnd;
	CString m_strSchemeName;
};
