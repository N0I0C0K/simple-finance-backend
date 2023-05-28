import { Box, Card, Divider, Stack, Typography } from '@mui/joy'
import { formatDateTime } from '../utils'
import { RecordProps, getRecordList } from '../utils/datafetch'
import React from 'react'

const RecordItem: React.FC<RecordProps> = (props) => {
  return (
    <Card variant='outlined'>
      <Box display={'flex'} alignItems={'center'}>
        <Typography level='h2' fontSize={'lg'}>
          {props.comment}
        </Typography>
        <Typography level='body3' sx={{ ml: 'auto' }}>
          {props.id.slice(0, 8)}
        </Typography>
      </Box>
      <Divider sx={{ mt: 2 }} />
      {/* <Typography level='body3'>{props.fromUserId.slice(0, 8)}</Typography> */}
      <Box display={'flex'} alignItems={'center'} mt={1}>
        <Box>
          <Typography level='body3'>amount</Typography>
          <Typography level='body1' fontSize={'20px'} fontStyle={'italic'}>
            ¥{props.money}
          </Typography>
        </Box>
        <Box ml={'auto'}>
          <Typography level='body3'>deal time</Typography>
          <Typography level='body2'>
            {formatDateTime(props.dealTime)}
          </Typography>
        </Box>
      </Box>
    </Card>
  )
}

export const Record: React.FC = () => {
  const [records, setRecords] = React.useState<RecordProps[]>([])
  React.useEffect(() => {
    getRecordList()
      .then((res) => {
        setRecords(res)
      })
      .catch((err) => {
        console.log(err)
      })
  }, [])
  return (
    <Stack gap={2}>
      <Typography level='h1'>Record</Typography>
      {records.map((record) => (
        <RecordItem {...record} key={record.id} />
      ))}
      <Box minHeight={'10vh'} />
    </Stack>
  )
}
