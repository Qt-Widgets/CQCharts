#include <CQChartsOptRect.h>

CQUTIL_DEF_META_TYPE(CQChartsOptRect, toString, fromString)

int CQChartsOptRect::metaTypeId;

void
CQChartsOptRect::
registerMetaType()
{
  metaTypeId = CQUTIL_REGISTER_META(CQChartsOptRect);
}
